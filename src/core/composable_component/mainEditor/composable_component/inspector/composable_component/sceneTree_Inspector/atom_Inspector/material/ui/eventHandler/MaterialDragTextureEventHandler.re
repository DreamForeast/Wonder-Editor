open DiffType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = Wonderjs.MaterialType.material;
  type dataTuple = int;
  let _handleSetMap =
      (gameObject, materialComponent, mapId, engineStateToGetData) =>
    switch (
      BasicMaterialEngineService.getMap(
        materialComponent,
        engineStateToGetData,
      )
    ) {
    | None =>
      OperateTextureLogicService.replaceMaterialComponentToHasMapOne(
        gameObject,
        materialComponent,
        mapId,
      )

    | Some(_map) =>
      OperateTextureLogicService.changeTextureMapAndRereshEngineState(
        materialComponent,
        mapId,
      )
    };
  /*
   todo implement when implement "import model" feature

   let handleCustomGeometryAddMap =
               (gameObject, materialComponent, mapId, engineStateToGetData) =>
             engineStateToGetData
             |> GameObjectComponentEngineService.getGeometryComponent(gameObject)
             |. GeometryEngineService.getCustomGeometryTexCoords(engineStateToGetData)
             |> GeometryService.hasTexCoords ?
               _handleSetMap(
                 gameObject,
                 materialComponent,
                 mapId,
                 engineStateToGetData,
               ) :
            WonderLog.Log.warn({j|the gameObject:$gameObject have no texCoords|j});
      */

  let _handleBoxGeometryAddMap =
      (gameObject, materialComponent, mapId, engineStateToGetData) =>
    engineStateToGetData
    |> GeometryEngineService.getBoxGeometryTexCoords
    |> GeometryService.hasTexCoords ?
      _handleSetMap(
        gameObject,
        materialComponent,
        mapId,
        engineStateToGetData,
      ) :
      WonderLog.Log.warn({j|the gameObject:$gameObject have no texCoords|j});

  let handleSelfLogic =
      ((store, dispatchFunc), materialComponent, dragedId) => {
    StateAssetService.getState()
    |> TextureNodeMapAssetService.getTextureNodeMap
    |> WonderCommonlib.SparseMapService.unsafeGet(dragedId)
    |> (
      ({textureIndex}) => {
        let gameObject =
          SceneEditorService.unsafeGetCurrentSceneTreeNode
          |> StateLogicService.getEditorState;

        let engineStateToGetData = StateLogicService.getRunEngineState();

        GameObjectEngineService.hasGameObjectBoxGeometryComponent(
          gameObject,
          engineStateToGetData,
        ) ?
          _handleBoxGeometryAddMap(
            gameObject,
            materialComponent,
            textureIndex,
            engineStateToGetData,
          ) :
          /* handleCustomGeometryAddMap(
               gameObject,
               materialComponent,
               result |> OptionService.unsafeGet |> int_of_string,
               engineStateToGetData,
             ); */
          ();
      }
    );
    dispatchFunc(AppStore.ReLoad) |> ignore;
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);