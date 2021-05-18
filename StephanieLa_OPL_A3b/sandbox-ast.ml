(* sandbox-ast.ml - T. Wilkes, Spring Semester 2021                   *)
(*                                                                    *)
(* The code in this file provides a "sandbox" environment for         *)
(* developing your AST functions for the Extended Calculator Language *)
(* in a step-by-step manner.                                          *)
(* This file is self-contained, i.e., you won't need to use any other *)
(* files while experimenting here.                                    *)
(* When you get a function working, copy-and-paste it into your       *)
(* interpreter.ml file.                                               *)
(* To open this file in ocaml or utop, at the toplevel prompt type:   *)
(*   #use "sandbox-ast.ml";;                                          *)

open List;;

(* Parse tree type from parser.ml *)
type parse_tree =
    | PT_error
    | PT_id of string
    | PT_num of string
    | PT_term of string
    | PT_nt of (string * parse_tree list);;

(* AST types from interpreter.ml *)
type ast_sl = ast_s list
and ast_s =
| AST_error
| AST_assign of (string * ast_e)
| AST_read of string
| AST_write of ast_e
| AST_if of (ast_c * ast_sl)
| AST_while of (ast_c * ast_sl)
and ast_e =
| AST_binop of (string * ast_e * ast_e)
| AST_id of string
| AST_num of string
and ast_c = (string * ast_e * ast_e);;

    
(* Some sample functions demonstrating parse tree pattern matching, *)
(* and building AST nodes                                           *)

let rec ast_ize_P (p:parse_tree) : ast_sl =
  match p with
  | PT_nt ("P", slist)    (* P -> SL $$ *)
    -> ast_ize_SL (hd slist)
    
  | _ -> raise (Failure "malformed parse tree in ast_ize_P")

and ast_ize_SL (sl:parse_tree) : ast_sl =
  match sl with
    | PT_nt ("SL", []) -> []   (* SL -> epsilon *)
    
    | PT_nt ("SL", head::tail) (* SL -> S SL *)
      -> [ast_ize_S head] @ (ast_ize_SL (hd tail))
      
    | _ -> raise (Failure "malformed parse tree in ast_ize_SL")

and ast_ize_S (s:parse_tree) : ast_s =
  match s with
    | PT_nt ("S", [PT_id lhs; PT_term ":="; expr])  (* S -> id := E *)
      -> AST_assign (lhs, (ast_ize_expr expr))
      
    | PT_nt ("S", [PT_term "read"; PT_id id])       (* S -> read id *)
      -> AST_read (id)

    | PT_nt ("S", [PT_term "write"; expr])	(* S -> write expr *)
      -> AST_write (ast_ize_expr expr)

    | PT_nt ("S", [PT_term "if"; c; sl; PT_term "end"])	(* S-> if expr *)
      -> AST_if ((ast_ize_C c), (ast_ize_SL sl))

    | PT_nt ("S", [PT_term "while"; c; sl; PT_term "end"])	(* S -> while expr *)
      -> AST_while((ast_ize_C c), (ast_ize_SL sl))

    | _ -> raise (Failure "malformed parse tree in ast_ize_S")

and ast_ize_expr (e:parse_tree) : ast_e =
  (* e is an E, T, or F parse tree node *)
  match e with
    | PT_nt ("E", [term; termtail])        (* E -> T TT *)
	-> ast_ize_expr_tail (ast_ize_expr (term)) termtail

    | PT_nt ("T", [factor; factortail])        (* T -> F FT *)
	-> ast_ize_expr_tail (ast_ize_expr (factor)) factortail 
 
    | PT_nt ("F", p)            (* F -> ( E ) | id | lit *)
    -> ( match p with
         | [PT_term "("; expr; PT_term ")"] -> ast_ize_expr(expr)
	 | [PT_id (id)] -> AST_id (id)
         | [PT_num (num)] -> AST_num (num) (* code for numeric literal *)
         | _ -> raise (Failure "malformed parse tree in ast_ize_expr")
       )
       
     | _ -> raise (Failure "malformed parse tree in ast_ize_expr")

and ast_ize_expr_tail (lhs:ast_e) (tail:parse_tree) : ast_e =
  (* lhs is an inherited attribute.
     tail is a TT or FT parse tree node *)
  match tail with
  | PT_nt ("TT", []) -> lhs	(* TT -> epsilon *)
  
  | PT_nt ("TT", [PT_nt ("ao", [PT_term "+"]); t; tt])	(* TT -> ao T TT *)                  
    -> ast_ize_expr_tail (AST_binop("+", lhs, ast_ize_expr t)) tt
  
  | PT_nt ("TT", [PT_nt ("ao", [PT_term "-"]); t; tt])	(* TT -> ao T TT *)                  
    -> ast_ize_expr_tail (AST_binop("-", lhs, ast_ize_expr t)) tt
  
  | PT_nt ("FT", []) -> lhs	(* FT -> epsilon *)

  | PT_nt ("FT", [PT_nt ("mo", [PT_term "*"]); f; ft])	(* FT -> mo F FT *)
  -> ast_ize_expr_tail (AST_binop("*", lhs, ast_ize_expr f)) ft
  
  | PT_nt ("FT", [PT_nt ("mo", [PT_term "/"]); f; ft])	(* FT -> mo F FT *)
  -> ast_ize_expr_tail (AST_binop("/", lhs, ast_ize_expr f)) ft
  
  | _ -> raise (Failure "malformed parse tree in ast_ize_expr_tail")

and ast_ize_C (c:parse_tree) : ast_c =
  match c with               (* C -> E ro E *)

  | PT_nt ("C", [lhs; PT_nt ("ro", [PT_term "<="]); rhs])
    -> ("<=", ast_ize_expr lhs, ast_ize_expr rhs)

  | PT_nt ("C", [lhs; PT_nt ("ro", [PT_term "=>"]); rhs])
    -> ("=>", ast_ize_expr lhs, ast_ize_expr rhs)

  | PT_nt ("C", [lhs; PT_nt ("ro", [PT_term ">"]); rhs])
    -> (">", ast_ize_expr lhs, ast_ize_expr rhs)

  | PT_nt ("C", [lhs; PT_nt ("ro", [PT_term "<"]); rhs])
    -> ("<", ast_ize_expr lhs, ast_ize_expr rhs)

  | PT_nt ("C", [lhs; PT_nt ("ro", [PT_term "<>"]); rhs])
    -> ("<>", ast_ize_expr lhs, ast_ize_expr rhs)

  | PT_nt ("C", [lhs; PT_nt ("ro", [PT_term "="]); rhs])
    -> ("=", ast_ize_expr lhs, ast_ize_expr rhs)

  | _ -> raise (Failure "malformed parse tree in ast_ize_C")
;;


(*******************************************************************
    Testing
 *******************************************************************)

(* "Hand build" a parse tree for part of the sum_ave_prog *)

let stmt1 = PT_nt ("S", [PT_term "read"; PT_id ("a")]);;

let stmt2 = PT_nt ("S", [PT_term "read"; PT_id ("b")]);;

let rhs = PT_nt ("E",
            [PT_nt ("T",
              [PT_nt ("F", [PT_id ("a")]);
               PT_nt ("FT", [])
              ]);
             PT_nt ("TT",
              [PT_nt ("ao", [PT_term ("+")]);
               PT_nt ("T",
                [PT_nt ("F", [PT_id ("b")]);
                 PT_nt ("FT", [])
                ]);
               PT_nt ("TT", [])
              ])
            ]);;
                
let stmt3 = PT_nt ("S", [PT_id ("sum"); PT_term (":="); rhs]);;

(* result of ast_ize_S stmt1;; *)
let stmt1_AST = AST_read "a";;

(* result of ast_ize_S stmt3;; *)
let stmt3_AST = AST_assign ("sum", AST_binop ("+", AST_id "a", AST_id "b"));;


(* A simple program using the if statement and a conditional expression *)
let if_prog = "
read x
if x > 0
  y := 100
end";;

let if_prog_PT = PT_nt ("P",
  [PT_nt
    ("SL",
     [PT_nt ("S", [PT_term "read"; PT_id "x"]);
      PT_nt
       ("SL",
        [PT_nt
          ("S",
           [PT_term "if";
            PT_nt
             ("C",
              [PT_nt
                ("E",
                 [PT_nt ("T", [PT_nt ("F", [PT_id "x"]); PT_nt ("FT", [])]);
                  PT_nt ("TT", [])]);
               PT_nt ("ro", [PT_term ">"]);
               PT_nt
                ("E",
                 [PT_nt ("T", [PT_nt ("F", [PT_num "0"]); PT_nt ("FT", [])]);
                  PT_nt ("TT", [])])]);
            PT_nt
             ("SL",
              [PT_nt
                ("S",
                 [PT_id "y"; PT_term ":=";
                  PT_nt
                   ("E",
                    [PT_nt
                      ("T", [PT_nt ("F", [PT_num "100"]); PT_nt ("FT", [])]);
                     PT_nt ("TT", [])])]);
               PT_nt ("SL", [])]);
            PT_term "end"]);
         PT_nt ("SL", [])])]);
   PT_term "$$"]);;

(* AST for if_prog: *)
let if_prog_AST =
  [AST_read "x";
   AST_if ((">", AST_id "x", AST_num "0"), [AST_assign ("y", AST_num "100")])]


(* A simple program using the while statement and a conditional expression *)
let while_prog = "
x := 0
while x <= 100
  x := x + 1
end";;

let while_prog_PT = PT_nt
 ("P",
  [PT_nt
    ("SL",
     [PT_nt
       ("S",
        [PT_id "x"; PT_term ":=";
         PT_nt
          ("E",
           [PT_nt ("T", [PT_nt ("F", [PT_num "0"]); PT_nt ("FT", [])]);
            PT_nt ("TT", [])])]);
      PT_nt
       ("SL",
        [PT_nt
          ("S",
           [PT_term "while";
            PT_nt
             ("C",
              [PT_nt
                ("E",
                 [PT_nt ("T", [PT_nt ("F", [PT_id "x"]); PT_nt ("FT", [])]);
                  PT_nt ("TT", [])]);
               PT_nt ("ro", [PT_term "<="]);
               PT_nt
                ("E",
                 [PT_nt ("T", [PT_nt ("F", [PT_num "100"]); PT_nt ("FT", [])]);
                  PT_nt ("TT", [])])]);
            PT_nt
             ("SL",
              [PT_nt
                ("S",
                 [PT_id "x"; PT_term ":=";
                  PT_nt
                   ("E",
                    [PT_nt ("T", [PT_nt ("F", [PT_id "x"]); PT_nt ("FT", [])]);
                     PT_nt
                      ("TT",
                       [PT_nt ("ao", [PT_term "+"]);
                        PT_nt
                         ("T", [PT_nt ("F", [PT_num "1"]); PT_nt ("FT", [])]);
                        PT_nt ("TT", [])])])]);
               PT_nt ("SL", [])]);
            PT_term "end"]);
         PT_nt ("SL", [])])]);
   PT_term "$$"]);;

(* AST for while_prog: *)
let while_prog_AST =
  [AST_assign ("x", AST_num "0");
   AST_while
    (("<=", AST_id "x", AST_num "100"),
     [AST_assign ("x", AST_binop ("+", AST_id "x", AST_num "1"))])];;
