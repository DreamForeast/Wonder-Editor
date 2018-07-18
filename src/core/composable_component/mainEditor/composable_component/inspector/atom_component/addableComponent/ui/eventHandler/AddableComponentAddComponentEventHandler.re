open DiffType;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;
  type prepareTuple = string;
  type dataTuple = Wonderjs.GameObjectType.gameObject;

  let handleSelfLogic = ((store, dispatchFunc), type_, currentSceneTreeNode) => {
    InspectorComponentUtils.addComponentByType(type_)
    |> StateLogicService.getAndRefreshEngineStateWithDiff([|
         {arguments: [|currentSceneTreeNode|], type_: GameObject},
       |]);
    dispatchFunc(AppStore.ReLoad) |> ignore;
  };
};

module MakeEventHandler =
  EventHandler.MakeEventHandler(CustomEventHandler);