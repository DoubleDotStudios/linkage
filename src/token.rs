#[derive(Debug, Clone)]
pub struct Token {
    pub value: String,
    pub kind: TokenKind,
}

#[derive(Debug, PartialEq, Clone)]
pub enum TokenKind {
    Number,     // 5
    Float,      // 5.5
    Identifier, // hello
    Return,
    Exit,
    Fn,
    Match,

    None,
    Empty,
    Null,
    True,
    False,

    // Symbols
    Eq,       // =
    Plus,     // +
    Minus,    // -
    Mul,      // *
    Div,      // /
    Mod,      // %
    Raise,    // ^
    Dollar,   // $
    At,       // @
    ExcPoint, // !
    QPoint,   // ?
    Pipe,     // |
    Comma,    // ,
    Dot,      // .
    DDot,     // ..
    Score,    // _
    FatArrow, // =>
    ArrowR,   // ->
    ArrowL,   // <-
    Hash,     // #
    MHash,    // #

    // Unary Operators
    Inc,    // ++
    Dec,    // --
    Sqr,    // ^^
    Sqrt,   // $$
    IntDiv, // //

    // Comparison
    NotEq, // !=
    LtEq,  // <=
    GtEq,  // >=
    EqEq,  // ==
    Or,    // ||
    And,   // &&

    // Brackets
    AngleR,   // >
    AngleL,   // <
    BraceR,   // }
    BraceL,   // {
    ParenR,   // )
    ParenL,   // (
    BracketR, // ]
    BracketL, // [

    // Special Symbols
    LangR, // ]|
    LangL, // |[

    EOF,
}
