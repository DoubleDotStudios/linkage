use std::process::exit;

pub fn verify_num(val: &str, msg: String) -> i32 {
    match val.parse::<i32>() {
        Ok(num) => return num,
        Err(_) => {
            eprintln!("{msg}");
            exit(1)
        }
    }
}
