open TreeAssetType;

open NodeAssetType;

let getNewName = () => "New IMGUICustomControl";

let isTreeIMGUICustomControlNodesHasTargetName = (name, tree) =>
  IterateTreeAssetService.find(
    ~tree,
    ~predIMGUISkinNodeFunc=
      node => IMGUICustomControlNodeAssetService.getNodeName(node) === name,
    (),
  )
  |> Js.Option.isSome;

let rename = (~name, ~nodeData): imguiCustomControlNodeData => {
  ...nodeData,
  name,
};