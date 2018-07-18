open AssetNodeType;
let handleSpeficFuncByAssetNodeType =
    (type_, (handleFodlerFunc, handleJsonFunc, handleTextureFunc)) =>
  switch (type_) {
  | Folder =>
    StateAssetService.getState()
    |> FolderNodeMapAssetService.getFolderNodeMap
    |> handleFodlerFunc
  | Texture =>
    StateAssetService.getState()
    |> TextureNodeMapAssetService.getTextureNodeMap
    |> handleTextureFunc
  | Json =>
    StateAssetService.getState()
    |> JsonNodeMapAssetService.getJsonNodeMap
    |> handleJsonFunc
  | _ =>
    WonderLog.Log.fatal(
      WonderLog.Log.buildFatalMessage(
        ~title="handleSpeficFuncByAssetNodeType",
        ~description={j|the type:$type_ not exist|j},
      ),
    )
  };