mod ast;
mod lexer;
mod list;
mod parser;
mod token;
mod utils;

use std::env;
use std::fs;
use std::process::exit;

use ast::AstNode;
use lexer::Lexer;
use parser::Parser;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        println!("Invalid use of llsc!\nExpected use: llsc <option> [flags]");
        exit(1)
    }

    match args[1].as_str() {
        "run" => run(),
        "build" => build(),
        _ => {
            println!("llsc: Invalid option: {}.", args[1]);
            exit(1);
        }
    }
}

fn build() {
    if let Ok(src) = fs::read_to_string("main.lk") {
        let lexer: Lexer = lexer::Lexer::new(&src);
        let mut parse: Parser = parser::Parser::new(lexer);
        let ast: AstNode = parser::parse(&mut parse);
    } else {
        eprintln!(
            "llsc: Unable to find 'main.lk'. Please make sure the file exists and try again.",
        );
        exit(1)
    }
}

fn run() {
    build();
}
