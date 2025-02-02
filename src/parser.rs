use std::process::exit;

use crate::{
    ast::{AstNode, AstType},
    lexer::{self, lex},
    list::List,
    token::{self, Token, TokenKind},
    utils,
};

pub struct Parser<'a> {
    lexer: lexer::Lexer<'a>,
    pub token: token::Token,
}

impl<'a> Parser<'a> {
    pub fn new(mut lexer: lexer::Lexer<'a>) -> Self {
        match lexer::lex(&mut lexer) {
            Ok(token) => return Self { lexer, token },
            Err(e) => {
                eprintln!("{e}");
                exit(1)
            }
        }
    }

    fn eat(&mut self, kind: TokenKind) -> Token {
        if self.token.kind != kind {
            eprintln!(
                "[Parser Error]: Unexpected token `{:?}`, expected `{:?}`",
                self.token.kind, kind
            );
            exit(1)
        }

        match lex(&mut self.lexer) {
            Ok(tok) => {
                self.token = tok.clone();
                return tok;
            }
            Err(e) => {
                eprintln!("{e}");
                exit(1)
            }
        }
    }

    fn expect_err(&mut self, expect: &str, expect_tk: TokenKind) {
        if self.token.kind != expect_tk {
            eprintln!(
                "[FATAL] [Parser Error]: Recieved `{}` when expecting `{}`",
                self.token.value, expect
            );
            exit(1)
        }
    }

    pub fn root(&mut self) -> AstNode {
        let mut root = AstNode::new(AstType::Root);

        match root.children {
            Some(ref mut children) => {
                while self.token.kind != TokenKind::EOF {
                    List::push(children, self.expr());
                }
            }
            None => {
                eprintln!("[FATAL] [Parser Error]: Invalid root node.");
                exit(1)
            }
        }

        root
    }

    fn expr(&mut self) -> AstNode {
        match self.token.kind {
            TokenKind::Identifier => self.identifier(),
            TokenKind::Exit => self.exit(),
            _ => {
                eprintln!("[Parser Error]: Invalid token `{}`", self.token.value);
                exit(1)
            }
        }
    }

    fn identifier(&mut self) -> AstNode {
        self.expect_err("Identifier", TokenKind::Identifier);

        let mut ast = AstNode::new(AstType::Identifier);
        ast.name = Some(self.token.value.clone());

        self.eat(TokenKind::Identifier);

        return ast;
    }

    fn exit(&mut self) -> AstNode {
        self.expect_err("Exit", TokenKind::Exit);

        let mut ast = AstNode::new(AstType::Invalid);

        self.eat(TokenKind::Exit);

        match self.token.kind {
            TokenKind::ArrowL => {
                ast.node_type = AstType::AssignExit;

                self.eat(TokenKind::ArrowL);

                let val = utils::verify_num(
                    &self.token.value,
                    format!(
                        "[Parser Error]: Expected integer value got `{}`",
                        self.token.value,
                    ),
                );

                ast.int_val = Some(val);

                self.eat(TokenKind::Number);
            }
            TokenKind::ParenL => {
                ast.node_type = AstType::ExecExit;

                self.eat(TokenKind::ParenL);
                self.eat(TokenKind::ParenR);
            }
            _ => {
                eprintln!(
                    "[Parser Error]: Unexpected token after `Exit` expected `(` or `<-`, got `{}`",
                    self.token.value
                );
                exit(1)
            }
        }

        return ast;
    }
}

pub fn parse<'a>(parser: &mut Parser<'a>) -> AstNode {
    return parser.root();
}
