use crate::list::List;

pub struct Visitor;

#[derive(Debug, PartialEq, Clone, Copy, Default)]
pub enum AstType {
    #[default]
    Invalid,

    Root,
    Compound,
    FnArgs,

    Identifier,
    AssignExit,
    ExecExit,
}

#[derive(Debug, Default, Clone)]
pub struct AstNode {
    pub node_type: AstType,

    pub name: Option<String>,
    pub children: Option<List>,
    pub data_type: Option<i32>,
    pub id: Option<i32>,

    pub val: Option<Box<AstNode>>,
    pub string_val: Option<String>,
    pub int_val: Option<i32>,
    pub args: Option<Box<AstNode>>,

    pub fptr: Option<fn(&mut Visitor, &AstNode, &Vec<AstNode>) -> Option<AstNode>>,
}

impl AstNode {
    pub fn new(node_type: AstType) -> Self {
        AstNode {
            node_type,
            ..Default::default()
        }
        .add_children(node_type)
    }

    fn add_children(mut self, node_type: AstType) -> Self {
        if node_type == AstType::Compound || node_type == AstType::Root {
            self.children = Some(List::new(std::mem::size_of::<*mut AstNode>()));
        }

        self
    }
}
