open NodeAssetType;

type tree =
  | ScriptEventFunctionNode(nodeId, scriptEventFunctionNodeData)
  | ScriptAttributeNode(nodeId, scriptAttributeNodeData)
  | TextureNode(nodeId, textureNodeData)
  | CubemapNode(nodeId, cubemapNodeData)
  | MaterialNode(nodeId, materialNodeData)
  | WDBNode(nodeId, wdbNodeData)
  | AssetBundleNode(nodeId, assetBundleNodeData)
  | FolderNode(
      nodeId,
      folderNodeData,
      UIStateAssetType.uiState(array(tree)),
    );