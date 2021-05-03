open Wonder_jest;

open Expect;

open Expect.Operators;

open Sinon;

open NodeAssetType;

open Js.Promise;

let _ =
  describe("MainEditorAssetChildrenNode", () => {
    let sandbox = getSandboxDefaultVal();

    beforeEach(() => {
      sandbox := createSandbox();
      MainEditorSceneTool.initState(~sandbox, ());

      MainEditorSceneTool.createDefaultScene(
        sandbox,
        MainEditorAssetTool.initAssetTree,
      );
      EventListenerTool.buildFakeDom()
      |> EventListenerTool.stubGetElementByIdReturnFakeDom;
    });
    afterEach(() => {
      restoreSandbox(refJsObjToSandbox(sandbox^));

      StateEditorService.getState()
      |> CurrentNodeIdAssetEditorService.clearCurrentNodeId
      |> SelectedFolderNodeIdInAssetTreeAssetEditorService.clearSelectedFolderNodeIdInAssetTree
      |> StateEditorService.setState
      |> ignore;
    });

    describe("test set current node", () => {
      test("click texture asset to be current node", () => {
        let assetTreeData =
          MainEditorAssetTreeTool.BuildAssetTree.Texture.buildOneTextureAssetTree();

        MainEditorAssetChildrenNodeTool.selectTextureNode(
          ~nodeId=
            MainEditorAssetTreeTool.BuildAssetTree.Texture.getFirstTextureNodeId(
              assetTreeData,
            ),
          (),
        );

        let currentNodeId =
          StateEditorService.getState()
          |> MainEditorAssetNodeTool.unsafeGetCurrentNodeId;

        currentNodeId
        |> expect
        == MainEditorAssetTreeTool.BuildAssetTree.Texture.getFirstTextureNodeId(
             assetTreeData,
           );
      });

      describe("test snapshot", () =>
        test("click fnt asset to be current node", () => {
          let assetTreeData =
            MainEditorAssetTreeTool.BuildAssetTree.buildEmptyAssetTree();

          let addedNodeId = MainEditorAssetIdTool.getNewAssetId();
          MainEditorAssetHeaderOperateNodeTool.addFnt();

          MainEditorAssetChildrenNodeTool.selectFntNode(
            ~nodeId=addedNodeId,
            (),
          );

          BuildComponentTool.buildAssetChildrenNode()
          |> ReactTestTool.createSnapshotAndMatch;
        })
      );
    });
    /*
      Jest.useFakeTimers will cause testPromise timeout(use Jest.useRealTimers not work)!!!

      describe("test click folder", () =>
        describe("test single click", () => {
          test("test set folder to be current node", () => {
            Jest.useFakeTimers();

            let assetTreeData =
              MainEditorAssetTreeTool.BuildAssetTree.Folder.TwoLayer.buildOneFolderAssetTree();

            let fakeDom =
              EventListenerTool.buildFakeDom()
              |> EventListenerTool.stubGetElementByIdReturnFakeDom;

            BuildComponentTool.buildAssetChildrenNode(~debounceTime=10, ());

            EventListenerTool.triggerEvent(fakeDom, "mousedown", {});

            Jest.runAllTimers();

            EventListenerTool.triggerEvent(fakeDom, "mousedown", {});

            let currentNodeId =
              StateEditorService.getState()
              |> MainEditorAssetNodeTool.unsafeGetCurrentNodeId;
            currentNodeId
            |>
            expect == (
                        assetTreeData
                        |> MainEditorAssetTreeTool.BuildAssetTree.Folder.TwoLayer.getFirstFolderNodeId
                      );
          });
          test("test snapshot", () => {
            Jest.useFakeTimers();
            let assetTreeData =
              MainEditorAssetTreeTool.BuildAssetTree.Folder.TwoLayer.buildOneFolderAssetTree();
            let fakeDom =
              EventListenerTool.buildFakeDom()
              |> EventListenerTool.stubGetElementByIdReturnFakeDom;
            BuildComponentTool.buildAssetChildrenNode(~debounceTime=10, ());

            EventListenerTool.triggerEvent(fakeDom, "mousedown", {});
            Jest.runAllTimers();
            EventListenerTool.triggerEvent(fakeDom, "mousedown", {});

            BuildComponentTool.buildAssetComponent()
            |> ReactTestTool.createSnapshotAndMatch;
          });
        })
      );
     testPromise(
          "double click folder, set folder to be currentAssetNodeParent and currentNode(are the same)",
          () => {
            Jest.useRealTimers();
            /* Jest.useFakeTimers(); */
            let assetTreeData =
              MainEditorAssetTreeTool.BuildAssetTree.Folder.TwoLayer.buildOneFolderAssetTree();

            let fakeDom =
              EventListenerTool.buildFakeDom()
              |> EventListenerTool.stubGetElementByIdReturnFakeDom;

            BuildComponentTool.buildAssetChildrenNode(~debounceTime=10, ());

            EventListenerTool.triggerEvent(fakeDom, "mousedown", {});

            /* Jest.runTimersToTime(5);

                                 EventListenerTool.triggerEvent(fakeDom, "mousedown", {});


               Jest.runTimersToTime(20);

                                     EventListenerTool.triggerEvent(
                                       fakeDom,
                                       "mousedown",
                                       {},
                                     );
                                         let editorState = StateEditorService.getState();

                                         NodeAssetService.isNodeEqualById(
                                           ~sourceNode=
                                             MainEditorAssetNodeTool.unsafeGetCurrentNode(
                                               editorState,
                                             ),
                                           ~targetNode=
                                             MainEditorAssetNodeTool.unsafeGetSelectedFolderNodeInAssetTree(
                                               editorState,
                                             ),
                                         )
                                         |> expect == true; */

            Js.Promise.make((~resolve, ~reject) =>
              TimeoutTool.setTimeout(
                () => {
                  EventListenerTool.triggerEvent(fakeDom, "mousedown", {});
                  TimeoutTool.setTimeout(
                    () => {
                      EventListenerTool.triggerEvent(
                        fakeDom,
                        "mousedown",
                        {},
                      );
                      resolve(.
                        {
                          let editorState = StateEditorService.getState();

                          NodeAssetService.isNodeEqualById(
                            ~sourceNode=
                              MainEditorAssetNodeTool.unsafeGetCurrentNode(
                                editorState,
                              ),
                            ~targetNode=
                              MainEditorAssetNodeTool.unsafeGetSelectedFolderNodeInAssetTree(
                                editorState,
                              ),
                          )
                          |> expect == true;
                        },
                      );
                    },
                    20,
                  );
                },
                5,
              )
            );
          },
        ); */

    describe("test asset tree node->isShowChildren", () => {
      describe("test double click folder", () =>
        test("folder->parent folder->isShowChildren should set to true", () => {
          let assetTreeData =
            MainEditorAssetTreeTool.BuildAssetTree.buildEmptyAssetTree();

          let addedFolderNodeId1 = MainEditorAssetIdTool.getNewAssetId();

          MainEditorAssetHeaderOperateNodeTool.addFolder();

          MainEditorAssetTreeTool.Select.selectFolderNode(
            ~nodeId=addedFolderNodeId1,
            (),
          );

          let addedFolderNodeId2 = addedFolderNodeId1 |> succ;

          MainEditorAssetHeaderOperateNodeTool.addFolder();

          MainEditorAssetTreeTool.Select.selectFolderNode(
            ~nodeId=addedFolderNodeId2,
            (),
          );

          let addedFolderNodeId3 = addedFolderNodeId2 |> succ;

          MainEditorAssetHeaderOperateNodeTool.addFolder();

          OperateTreeAssetEditorService.setNodeIsShowChildren(
            addedFolderNodeId1,
            false,
          )
          |> StateLogicService.getAndSetEditorState;

          FolderBoxTool.onDoubleClick(~nodeId=addedFolderNodeId2, ());

          FolderNodeAssetService.getIsShowChildren(
            OperateTreeAssetEditorService.unsafeFindNodeById(
              addedFolderNodeId1,
              StateEditorService.getState(),
            ),
          )
          |> expect == true;
        })
      );

      describe("render material sphere's snapshot", () => {
        describe("render default snapshot", () =>
          describe("test render after add material", () =>
            test("test snapshot", () => {
              MainEditorAssetTreeTool.BuildAssetTree.buildEmptyAssetTree()
              |> ignore;

              MainEditorAssetHeaderOperateNodeTool.addMaterial();

              BuildComponentTool.buildAssetChildrenNode()
              |> ReactTestTool.createSnapshotAndMatch;
            })
          )
        );

        describe("render updated snapshot", () => {
          beforeEach(() => {
            MainEditorSceneTool.initInspectorEngineState(
              ~sandbox,
              ~isInitJob=false,
              ~noWorkerJobRecord=
                NoWorkerJobConfigToolEngine.buildNoWorkerJobConfig(
                  ~initPipelines=
                    {|
             [
              {
                "name": "default",
                "jobs": [
                    {"name": "init_inspector_engine" }
                ]
              }
            ]
             |},
                  ~initJobs=
                    {|
             [
                {"name": "init_inspector_engine" }
             ]
             |},
                  (),
                ),
              (),
            );

            StateInspectorEngineService.unsafeGetState()
            |> MainUtils._handleInspectorEngineState
            |> StateInspectorEngineService.setState
            |> ignore;

            MainEditorSceneTool.createDefaultScene(
              sandbox,
              MainEditorSceneTool.setFirstCubeToBeCurrentSceneTreeNode,
            );

            MainEditorAssetTool.buildFakeImage();
            MainEditorAssetTool.buildFakeFileReader();
          });

          describe("test render after drag texture", () =>
            testPromise("test snapshot", () => {
              let (
                addedMaterialNodeId,
                newMaterialComponent,
                imgCanvasFakeBase64Str,
                (inspectorCanvasDom, imgCanvasDom),
              ) =
                MainEditorLightMaterialForAssetTool.prepareInspectorMaterialSphereAndImgCanvas(
                  ~sandbox,
                  (),
                );

              MainEditorAssetUploadTool.loadOneTexture()
              |> Js.Promise.then_(uploadedTextureNodeId => {
                   MainEditorLightMaterialForAssetTool.dragAssetTextureToMap(
                     ~currentNodeId=addedMaterialNodeId,
                     ~textureNodeId=uploadedTextureNodeId,
                     ~material=newMaterialComponent,
                     (),
                   );

                   BuildComponentTool.buildAssetChildrenNode()
                   |> ReactTestTool.createSnapshotAndMatch
                   |> resolve;
                 });
            })
          );
        });
      });
    });

    describe("test show order", () => {
      let boxTexturedWDBArrayBuffer = ref(Obj.magic(1));

      beforeAll(() =>
        boxTexturedWDBArrayBuffer := WDBTool.convertGLBToWDB("BoxTextured")
      );

      beforeEach(() => {
        MainEditorAssetTool.buildFakeFileReader();

        LoadTool.buildFakeTextDecoder(LoadTool.convertUint8ArrayToBuffer);
        LoadTool.buildFakeURL(sandbox^);

        LoadTool.buildFakeLoadImage(.);

        MainEditorAssetTool.buildFakeImage();
      });

      testPromise(
        {|
        order should be:
        1)for different type_:folder,wdb,material,texture;
        2)for the same type_:sort by firstname alphabetically
        |},
        () => {
          let addedFolderNodeId1 = MainEditorAssetIdTool.getNewAssetId();
          MainEditorAssetHeaderOperateNodeTool.addFolder();

          let addedMaterialNodeId1 = addedFolderNodeId1 |> succ;
          MainEditorAssetHeaderOperateNodeTool.addMaterial();

          let addedFolderNodeId2 = addedMaterialNodeId1 |> succ;
          MainEditorAssetHeaderOperateNodeTool.addFolder();

          let addedMaterialNodeId2 = addedFolderNodeId2 |> succ;
          MainEditorAssetHeaderOperateNodeTool.addMaterial();

          let wdbName1 = "C_WDB";

          MainEditorAssetUploadTool.loadOneWDB(
            ~fileName=wdbName1,
            ~arrayBuffer=boxTexturedWDBArrayBuffer^,
            (),
          )
          |> then_(uploadedWDBNodeId1 => {
               let imgName1 = "BImage.png";
               MainEditorAssetUploadTool.loadOneTexture(~imgName=imgName1, ())
               |> then_(uploadedTextureNodeId1 => {
                    let wdbName2 = "A_WDB";

                    MainEditorAssetUploadTool.loadOneWDB(
                      ~fileName=wdbName2,
                      ~arrayBuffer=boxTexturedWDBArrayBuffer^,
                      (),
                    )
                    |> then_(uploadedWDBNodeId2 => {
                         let imgName2 = "AImage.jpg";
                         MainEditorAssetUploadTool.loadOneTexture(
                           ~imgName=imgName2,
                           (),
                         )
                         |> then_(uploadedTextureNodeId2 => {
                              AssetInspectorTool.Rename.renameAssetFolderNode(
                                ~nodeId=addedFolderNodeId1,
                                ~name="FFolder",
                                (),
                              );
                              AssetInspectorTool.Rename.renameAssetFolderNode(
                                ~nodeId=addedFolderNodeId2,
                                ~name="AFolder",
                                (),
                              );

                              AssetInspectorTool.Rename.renameAssetMaterialNode(
                                ~nodeId=addedMaterialNodeId1,
                                ~name="CMaterial",
                                (),
                              );
                              AssetInspectorTool.Rename.renameAssetMaterialNode(
                                ~nodeId=addedMaterialNodeId2,
                                ~name="AMaterial",
                                (),
                              );

                              BuildComponentTool.buildAssetChildrenNode()
                              |> ReactTestTool.createSnapshotAndMatch
                              |> resolve;
                            });
                       });
                  });
             });
        },
      );
    });
  });