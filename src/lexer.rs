use std::char;

use crate::token::{Token, TokenKind};

pub struct Lexer<'a> {
    src: &'a str,
    src_size: usize,
    i: usize,
    c: char,
}

impl<'a> Lexer<'a> {
    pub fn new(src: &'a str) -> Self {
        let src_size = src.len();
        let c = src.chars().next().unwrap_or('\0');

        Self {
            src,
            src_size,
            i: 0,
            c,
        }
    }

    pub fn advance(&mut self) {
        if self.i < self.src_size && self.c != '\0' {
            self.i += 1;
            self.c = self.src.chars().nth(self.i).unwrap_or('\0');
        }
    }

    pub fn advance_with(&mut self, value: &str, kind: TokenKind) -> Token {
        self.advance();
        return Token {
            value: value.to_string(),
            kind,
        };
    }

    pub fn peek(&mut self, offset: usize) -> Option<char> {
        let index = std::cmp::min(self.i + offset, self.src.len());
        self.src.chars().nth(index)
    }

    pub fn skip_whitespace(&mut self) {
        while self.c == '\t' || self.c == '\n' || self.c == '\r' || self.c == ' ' {
            self.advance()
        }
    }

    pub fn skip_comment(&mut self) {
        self.advance();

        while self.c != '\n' && self.c != '\r' {
            self.advance();
        }
    }

    pub fn skip_multiline(&mut self) {
        self.advance();

        while self.c != '#' && self.peek(1) != Some('#') {
            self.advance();
        }

        self.advance();
        self.advance();
    }

    pub fn parse_number(&mut self) -> Token {
        let mut val = Vec::new();
        let mut tk = TokenKind::Number;

        while self.c.is_numeric() {
            val.push(self.c);
            self.advance();
        }

        if self.c == '.' {
            tk = TokenKind::Float;
            val.push(self.c);
            self.advance();

            while self.c.is_numeric() {
                val.push(self.c);
                self.advance();
            }
        }

        Token {
            value: String::from_iter(val),
            kind: tk,
        }
    }

    pub fn parse_identifier(&mut self) -> Token {
        let tk;
        let mut val = Vec::new();

        while self.c.is_alphabetic() {
            val.push(self.c);
            self.advance();
        }

        let str_val = String::from_iter(val);

        match str_val.as_str() {
            "exit" => tk = TokenKind::Exit,
            "return" => tk = TokenKind::Return,
            "fn" => tk = TokenKind::Fn,
            "match" => tk = TokenKind::Match,

            "none" => tk = TokenKind::None,
            "empty" => tk = TokenKind::Empty,
            "null" => tk = TokenKind::Null,
            "true" => tk = TokenKind::True,
            "false" => tk = TokenKind::False,

            _ => tk = TokenKind::Identifier,
        }

        Token {
            value: str_val,
            kind: tk,
        }
    }
}

pub fn lex<'a>(lexer: &mut Lexer<'a>) -> Result<Token, String> {
    while lexer.c != '\0' {
        lexer.skip_whitespace();

        if lexer.c.is_numeric() {
            return Ok(lexer.parse_number());
        }

        if lexer.c.is_alphabetic() {
            return Ok(lexer.parse_identifier());
        }

        match lexer.c {
            '=' => {
                if lexer.peek(1) == Some('=') {
                    lexer.advance();
                    return Ok(lexer.advance_with("==", TokenKind::EqEq));
                } else if lexer.peek(1) == Some('>') {
                    lexer.advance();
                    return Ok(lexer.advance_with("=>", TokenKind::FatArrow));
                } else {
                    return Ok(lexer.advance_with("=", TokenKind::Eq));
                }
            }
            '+' => {
                if lexer.peek(1) == Some('+') {
                    lexer.advance();
                    return Ok(lexer.advance_with("++", TokenKind::Inc));
                } else {
                    return Ok(lexer.advance_with("+", TokenKind::Plus));
                }
            }
            '-' => {
                if lexer.peek(1) == Some('-') {
                    lexer.advance();
                    return Ok(lexer.advance_with("--", TokenKind::Dec));
                } else if lexer.peek(1) == Some('>') {
                    lexer.advance();
                    return Ok(lexer.advance_with("->", TokenKind::ArrowR));
                } else {
                    return Ok(lexer.advance_with("-", TokenKind::Minus));
                }
            }
            '/' => {
                if lexer.peek(1) == Some('/') {
                    lexer.advance();
                    return Ok(lexer.advance_with("//", TokenKind::IntDiv));
                } else {
                    return Ok(lexer.advance_with("/", TokenKind::Div));
                }
            }
            '^' => {
                if lexer.peek(1) == Some('^') {
                    lexer.advance();
                    return Ok(lexer.advance_with("^^", TokenKind::Sqr));
                } else {
                    return Ok(lexer.advance_with("^", TokenKind::Raise));
                }
            }
            '$' => {
                if lexer.peek(1) == Some('$') {
                    lexer.advance();
                    return Ok(lexer.advance_with("$$", TokenKind::Sqrt));
                } else {
                    return Ok(lexer.advance_with("$", TokenKind::Dollar));
                }
            }
            '*' => return Ok(lexer.advance_with("*", TokenKind::Mul)),
            '%' => return Ok(lexer.advance_with("%", TokenKind::Mod)),

            '@' => return Ok(lexer.advance_with("@", TokenKind::At)),
            '?' => return Ok(lexer.advance_with("?", TokenKind::QPoint)),
            ',' => return Ok(lexer.advance_with(",", TokenKind::Comma)),
            '_' => return Ok(lexer.advance_with("_", TokenKind::Score)),
            '!' => {
                if lexer.peek(1) == Some('=') {
                    lexer.advance();
                    return Ok(lexer.advance_with("!=", TokenKind::NotEq));
                } else {
                    return Ok(lexer.advance_with("!", TokenKind::ExcPoint));
                }
            }
            '|' => {
                if lexer.peek(1) == Some('|') {
                    lexer.advance();
                    return Ok(lexer.advance_with("||", TokenKind::Or));
                } else if lexer.peek(1) == Some('[') {
                    lexer.advance();
                    return Ok(lexer.advance_with("|[", TokenKind::LangL));
                } else {
                    return Ok(lexer.advance_with("|", TokenKind::Pipe));
                }
            }
            '&' => {
                if lexer.peek(1) == Some('&') {
                    lexer.advance();
                    return Ok(lexer.advance_with("&&", TokenKind::And));
                } else {
                    return Err(format!(
                        "[Lexer Error]: Unable to parse symbol: {}",
                        lexer.c
                    ));
                }
            }
            '.' => {
                if lexer.peek(1) == Some('.') {
                    lexer.advance();
                    return Ok(lexer.advance_with("..", TokenKind::DDot));
                } else {
                    return Ok(lexer.advance_with(".", TokenKind::Dot));
                }
            }
            '#' => {
                if lexer.peek(1) == Some('#') {
                    lexer.advance();
                    lexer.skip_multiline();
                } else {
                    lexer.skip_comment();
                }
            }

            '>' => {
                if lexer.peek(1) == Some('=') {
                    lexer.advance();
                    return Ok(lexer.advance_with(">=", TokenKind::GtEq));
                } else {
                    return Ok(lexer.advance_with(">", TokenKind::AngleR));
                }
            }
            '<' => {
                if lexer.peek(1) == Some('=') {
                    lexer.advance();
                    return Ok(lexer.advance_with("<=", TokenKind::LtEq));
                } else if lexer.peek(1) == Some('-') {
                    lexer.advance();
                    return Ok(lexer.advance_with("<-", TokenKind::ArrowL));
                } else {
                    return Ok(lexer.advance_with("<", TokenKind::AngleL));
                }
            }
            '}' => return Ok(lexer.advance_with("}", TokenKind::BraceR)),
            '{' => return Ok(lexer.advance_with("{", TokenKind::BraceL)),
            ']' => {
                if lexer.peek(1) == Some('|') {
                    lexer.advance();
                    return Ok(lexer.advance_with("]|", TokenKind::LangR));
                } else {
                    return Ok(lexer.advance_with("]", TokenKind::BracketR));
                }
            }
            '[' => return Ok(lexer.advance_with("[", TokenKind::BracketL)),
            ')' => return Ok(lexer.advance_with(")", TokenKind::ParenR)),
            '(' => return Ok(lexer.advance_with("(", TokenKind::ParenL)),

            '\0' => break,

            _ => {
                return Err(format!(
                    "[Lexer Error]: Unable to parse symbol: {}",
                    lexer.c
                ));
            }
        }
    }

    return Ok(Token {
        value: 0.to_string(),
        kind: TokenKind::EOF,
    });
}
