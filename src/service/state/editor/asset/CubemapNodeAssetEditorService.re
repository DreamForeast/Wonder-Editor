/* let setNodeData = (nodeId, nodeData, editorState) =>
     NodeAssetEditorService.setNodeData(
       nodeId,
       nodeData,
       CubemapNodeAssetService.buildNodeByNodeData,
       editorState,
     );

   let unsafeGetNodeData = (nodeId, editorState) =>
     OperateTreeAssetEditorService.unsafeFindNodeById(nodeId, editorState)
     |> CubemapNodeAssetService.getNodeData; */

let addCubemapNodeToAssetTree = (targetTreeNode, newNode, editorState) =>
  NodeAssetEditorService.addNodeToAssetTree(
    targetTreeNode,
    newNode,
    editorState,
  );

let findAllCubemapNodes = editorState =>
  IterateTreeAssetEditorService.filter(
    ~acc=[||],
    ~pushNodeFunc=(node, acc) => acc |> ArrayService.push(node),
    ~editorState,
    ~predCubemapNodeFunc=node => true,
    (),
  );

/* let getDataByCubemapComponent = (targetCubemapComponent, editorState) =>
     IterateTreeAssetService.findOne(
       ~tree=TreeAssetEditorService.unsafeGetTree(editorState),
       ~predCubemapNodeFunc=
         node => {
           let {textureComponent}: NodeAssetType.textureNodeData =
             CubemapNodeAssetService.getNodeData(node);

           textureComponent === targetCubemapComponent;
         },
       (),
     )
     |> Js.Option.map((. node) => CubemapNodeAssetService.getNodeData(node));

   let findAllCubemapNodes = editorState =>
     IterateTreeAssetEditorService.filter(
       ~acc=[||],
       ~pushNodeFunc=(node, acc) => acc |> ArrayService.push(node),
       ~editorState,
       ~predCubemapNodeFunc=node => true,
       (),
     );

   let getCubemapComponents = editorState =>
     editorState
     |> findAllCubemapNodes
     |> Js.Array.map(node => {
          let {textureComponent}: NodeAssetType.textureNodeData =
            CubemapNodeAssetService.getNodeData(node);

          textureComponent;
        }); */