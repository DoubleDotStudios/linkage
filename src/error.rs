use std::process::exit;

#[derive(Debug)]
pub enum ErrLevel {
    Note,
    Warn,
    Error,
    Fatal,
}

pub struct Err {
    msg: String,
    level: ErrLevel,
    line: i32,
    code: i32,
}

impl Err {
    pub fn new(msg: String, level: ErrLevel, line: i32, code: i32) -> Self {
        Err {
            msg,
            level,
            line,
            code,
        }
    }

    pub fn fmt(err: Err) -> () {
        let line: i32 = err.line;
        let level: String = convert_level(err.level);

        let mut prev: String = (line - 1).to_string();
        let mut curr: String = line.to_string();
        let mut next: String = (line + 1).to_string();

        let prev_len: usize = prev.len();
        let curr_len: usize = curr.len();
        let next_len: usize = next.len();

        let max_len: usize = prev_len.max(curr_len).max(next_len);

        if prev_len < max_len {
            prev = format!("{:>width$}", prev, width = max_len);
        }
        if curr_len < max_len {
            curr = format!("{:>width$}", curr, width = max_len);
        }
        if next_len < max_len {
            next = format!("{:>width$}", next, width = max_len);
        }

        let msg: String = format!(
            "[{}{}] {}\n  {}| {}\n* {}| {}\n{}| {}\n",
            level, 1, err.msg, prev, "hi", curr, "olleh", next, "hello"
        );

        eprintln!("{}", msg);
    }
}

fn convert_level(level: ErrLevel) -> String {
    match level {
        ErrLevel::Note => return "N".to_string(),
        ErrLevel::Warn => return "W".to_string(),
        ErrLevel::Error => return "E".to_string(),
        ErrLevel::Fatal => return "F".to_string(),
    }
}

// TARGET:
// [E<NUM>] Expected ')', got '}':
//    8| ;x = 24
// *  9| ;y = (32 + x}
//   10| exit <- y
