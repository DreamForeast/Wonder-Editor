let setNodeData = (nodeId, nodeData, editorState) =>
  NodeAssetEditorService.setNodeData(
    nodeId,
    nodeData,
    TextureNodeAssetService.buildNodeByNodeData,
    editorState,
  );

let unsafeGetNodeData = (nodeId, editorState) =>
  OperateTreeAssetEditorService.unsafeFindNodeById(nodeId, editorState)
  |> TextureNodeAssetService.getNodeData;

let addTextureNodeToAssetTree = (targetTreeNode, newNode, editorState) =>
  NodeAssetEditorService.addNodeToAssetTree(
    targetTreeNode,
    newNode,
    editorState,
  );

let doesAnyTextureUseImage = (targetImageDataIndex, editorState) =>
  IterateTreeAssetService.filter(
    ~acc=[],
    ~pushNodeFunc=(node, acc) => [node, ...acc],
    ~tree=TreeAssetEditorService.unsafeGetTree(editorState),
    ~predTextureNodeFunc=
      node => {
        let {imageDataIndex}: NodeAssetType.textureNodeData =
          TextureNodeAssetService.getNodeData(node);

        imageDataIndex === targetImageDataIndex;
      },
    (),
  )
  |> List.length > 1;

let getDataByTextureComponent = (targetTextureComponent, editorState) =>
  IterateTreeAssetService.findOne(
    ~tree=TreeAssetEditorService.unsafeGetTree(editorState),
    ~predTextureNodeFunc=
      node => {
        let {textureComponent}: NodeAssetType.textureNodeData =
          TextureNodeAssetService.getNodeData(node);

        textureComponent === targetTextureComponent;
      },
    (),
  )
  |> Js.Option.map((. node) => TextureNodeAssetService.getNodeData(node));

let _findAllTextureNodes = editorState =>
  IterateTreeAssetEditorService.filter(
    ~acc=[||],
    ~pushNodeFunc=(node, acc) => acc |> ArrayService.push(node),
    ~editorState,
    ~predTextureNodeFunc=node => true,
    (),
  );

let findAllTextureNodesByType = (isTypeFunc, editorState) =>
  _findAllTextureNodes(editorState)
  |> Js.Array.filter(node => isTypeFunc(node));

let getTextureComponents = nodes =>
  nodes
  |> Js.Array.map(node => {
       let {textureComponent}: NodeAssetType.textureNodeData =
         TextureNodeAssetService.getNodeData(node);

       textureComponent;
     });

let getType = (nodeId, editorState) =>
  OperateTreeAssetEditorService.unsafeFindNodeById(nodeId, editorState)
  |> TextureNodeAssetService.getType;

let getImageDataIndex = (nodeId, editorState) =>
  OperateTreeAssetEditorService.unsafeFindNodeById(nodeId, editorState)
  |> TextureNodeAssetService.getImageDataIndex;