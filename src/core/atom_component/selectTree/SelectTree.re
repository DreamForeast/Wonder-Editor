open SelectTreeType;

module Method = {
  let _hasChildren = node =>
    FolderNodeSelectTreeService.isFolderNode(node) ?
      FolderNodeSelectTreeService.getChildren(node) |> Js.Array.length > 0 :
      false;

  let _isSelected = node =>
    switch (node) {
    | FolderNode(_, nodeData, _) => nodeData.isSelect
    | ValueNode(_, nodeData) => nodeData.isSelect
    };

  let _toggleSelect = (event, node, toggleSelectFunc) => {
    let checked =
      ReactDOMRe.domElementToObj(ReactEventRe.Mouse.target(event))##checked;

    toggleSelectFunc(checked, node);
  };

  let _getIcon = (node, getValueNodeIconFunc, editorState) =>
    switch (node) {
    | FolderNode(_, _, _) => Some("./public/img/package.png")
    | ValueNode(_, nodeData) =>
      getValueNodeIconFunc(nodeData.type_, nodeData.value, editorState)
    };

  let rec _build = (allNodes, (getValueNodeIconFunc, toggleSelectFunc)) =>
    allNodes
    /* |> _sortByName */
    |> Js.Array.map(node =>
         <ul
           className="wonder-tree-node"
           key={
             StringService.intToString(NodeSelectTreeService.getNodeId(node))
           }>
           <li>
             {
               _hasChildren(node) ?
                 <div className="item-triangle">
                   <img src="./public/img/down.png" />
                 </div> :
                 <div className="item-triangle" />
             }
             <div className="select-box">
               <input
                 type_="checkbox"
                 defaultChecked={_isSelected(node)}
                 onClick={e => _toggleSelect(e, node, toggleSelectFunc)}
               />
             </div>
             {
               switch (
                 _getIcon(node, getValueNodeIconFunc)
                 |> StateLogicService.getEditorState
               ) {
               | None => ReasonReact.null
               | Some(icon) => <img src=icon className="treeNode-icon" />
               }
             }
             <span>
               {DomHelper.textEl(NodeSelectTreeService.getNodeName(node))}
             </span>
           </li>
           {
             ReasonReact.array(
               _build(
                 _hasChildren(node) ?
                   FolderNodeSelectTreeService.getChildren(node) : [||],
                 (getValueNodeIconFunc, toggleSelectFunc),
               ),
             )
           }
         </ul>
       );

  let buildTreeArray = (tree, (getValueNodeIconFunc, toggleSelectFunc)) =>
    _build(
      [|RootTreeSelectTreeService.getRootNode(tree)|],
      (getValueNodeIconFunc, toggleSelectFunc),
    );
};

let component = ReasonReact.statelessComponent("SelectTree");

let render = (tree: tree, (getValueNodeIconFunc, toggleSelectFunc), _self) =>
  <article key="selectTreeRoot" className="wonder-selectTree">
    {
      ReasonReact.array(
        Method.buildTreeArray(
          tree,
          (getValueNodeIconFunc, toggleSelectFunc),
        ),
      )
    }
  </article>;

let make = (~tree, ~getValueNodeIconFunc, ~toggleSelectFunc, _children) => {
  ...component,
  render: self =>
    render(tree, (getValueNodeIconFunc, toggleSelectFunc), self),
};