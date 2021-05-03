open EditorType;

let getMap = editorState => editorState.assetRecord.basicSourceTextureImageDataMap;

let setMap = (map, editorState) => {
  ...editorState,
  assetRecord: {
    ...editorState.assetRecord,
    basicSourceTextureImageDataMap: map,
  },
};

let clearMap = editorState => {
  ...editorState,
  assetRecord: {
    ...editorState.assetRecord,
    basicSourceTextureImageDataMap: WonderCommonlib.ImmutableSparseMapService.createEmpty(),
  },
};

let getData = (index, editorState) =>
  BasicSourceTextureImageDataMapAssetService.getData(index, editorState.assetRecord);

let unsafeGetData = (index, editorState) =>
  BasicSourceTextureImageDataMapAssetService.unsafeGetData(index, editorState.assetRecord);

let setData = (index, data, editorState) => {
  ...editorState,
  assetRecord:
    editorState.assetRecord |> BasicSourceTextureImageDataMapAssetService.setData(index, data),
};

let _getImageDataIndexByBase64 = (imageBase64, editorState) =>
  switch (
    editorState
    |> getMap
    |> WonderCommonlib.ImmutableSparseMapService.getValidDataArr
    |> Js.Array.find(((imageDataIndex, {base64}: ImageDataType.imageData)) =>
         Base64Service.isBase64Equal(Some(imageBase64), base64)
       )
  ) {
  | None => None
  | Some((imageDataIndex, _)) => Some(imageDataIndex)
  };

let addImageDataIfBase64NotExist = (base64, fileName, mimeType, editorState) =>
  switch (_getImageDataIndexByBase64(base64, editorState)) {
  | None =>
    let (editorState, newImageDataIndex) =
      IndexAssetEditorService.generateBasicSourceTextureImageDataMapIndex(editorState);

    (
      editorState
      |> setData(
           newImageDataIndex,
           BasicSourceTextureImageDataMapAssetService.buildData(
             ~base64=Some(base64),
             ~uint8Array=None,
             ~name=fileName,
             ~mimeType,
             (),
           ),
         ),
      newImageDataIndex,
    );
  | Some(imageDataIndex) => (editorState, imageDataIndex)
  };

let _getImageDataIndexByUint8Array = (imageUint8Array, editorState) =>
  switch (
    editorState
    |> getMap
    |> WonderCommonlib.ImmutableSparseMapService.getValidDataArr
    |> Js.Array.find(
         ((imageDataIndex, {uint8Array}: ImageDataType.imageData)) =>
         Uint8ArrayService.isUint8ArrayEqual(
           Some(imageUint8Array),
           uint8Array,
         )
       )
  ) {
  | None => None
  | Some((imageDataIndex, _)) => Some(imageDataIndex)
  };

let addImageDataIfUint8ArrayNotExist =
    (uint8Array, name, mimeType, editorState) =>
  switch (_getImageDataIndexByUint8Array(uint8Array, editorState)) {
  | None =>
    let (editorState, newImageDataIndex) =
      IndexAssetEditorService.generateBasicSourceTextureImageDataMapIndex(editorState);

    (
      editorState
      |> setData(
           newImageDataIndex,
           BasicSourceTextureImageDataMapAssetService.buildData(
             ~base64=None,
             ~uint8Array=Some(uint8Array),
             ~name,
             ~mimeType,
             (),
           ),
         ),
      newImageDataIndex,
    );
  | Some(imageDataIndex) => (editorState, imageDataIndex)
  };

let removeData = (index, editorState) => {
  ...editorState,
  assetRecord:
    editorState.assetRecord |> BasicSourceTextureImageDataMapAssetService.removeData(index),
};

let unsafeGetUint8Array = (index, editorState) =>
  unsafeGetData(index, editorState).uint8Array;

let getValidValues = editorState =>
  getMap(editorState)
  |> WonderCommonlib.ImmutableSparseMapService.getValidValues;