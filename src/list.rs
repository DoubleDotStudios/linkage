use crate::ast::AstNode;

#[derive(Debug, Clone)]
pub struct List {
    pub items: Vec<AstNode>,
    pub item_size: usize,
}

impl List {
    pub fn new(item_size: usize) -> Self {
        Self {
            item_size,
            items: Vec::new(),
        }
    }

    pub fn push(&mut self, item: AstNode) {
        self.items.push(item);
    }

    pub fn size(&self) -> usize {
        self.items.len()
    }
}
