//Not separately compiled - included directly!

#include "CannedLevel.h"

namespace{
    struct SwitchData{
        RegionSensor* sensor;
        SharedDynamic<bool> on;
    };
    SwitchData createSwitch(GameState& state, Rectangle<double> const& region, Rectangle<double> const& base){
        SwitchData ret;
        ret.sensor = state.addRegion(region);
        ret.on->set(false);
        state.registration.renderers.push_back(SensorRenderers::SwitchGen{
            ret.sensor,
            ret.on,
            base
        });
        auto player = [on = ret.on](GameState& state){
            if(!on->get()){
                on->set(true);
                state.waitingFuncs.push_back([](Window& window, ImmediateData& data){
                    window.playSample(data.click, 0.1f);
                });
            }
        };
        state.registration.playerActions.insert(std::make_pair(ret.sensor, PlayerActions::Special{
            [player](GameState& state, double dt, Player*){ player(state); }
        }));
        state.registration.escortActions.insert(std::make_pair(ret.sensor, EscortActions::Special{
            [player](GameState& state, double dt, Escort*){ player(state); }
        }));
        return ret;
    }
    SolidObject* linkBlockToSwitch(GameState& state, SwitchData& bridgeSwitch, Rectangle<double> const& region, bool rev = false){
        auto bridge = state.addBlock(region, rev, rev);
        bridgeSwitch.on->updated.hook([&state, bridge, rev](bool on){
            bridge->canHitCircle = on != rev;
            bridge->canHitSquare = on != rev;
        });
        auto bridgeGhost = state.addRegion(bridge->region);
        state.registration.renderers.push_back(SensorRenderers::Ghost::Gen{bridgeGhost});
        return bridge;
    }
}
LevelDescription const& getLevel(std::size_t index){
    if(index == 0){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            //Top wall
            desc.items.push_back(BlockDescription{{{-2, 0.9}, {2, 1}}});

            //Left wall
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}});

            //Top row!
            desc.items.push_back(BlockDescription{{{-1, 0.3}, {-0.1, 0.4}}});
            desc.items.push_back(BlockDescription{{{0.1, -1}, {0.7, 0.4}}});
            
            //Safe pocket
            desc.items.push_back(BlockDescription{{{0.7, -1}, {0.9, 0.1}}});
            desc.items.push_back(BlockDescription{{{0.9, -1}, {2, 1}}});
            desc.items.push_back(SensorDescription{{{0.7, 0.1}, {0.9, 0.35}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});

            //Bottom row
            desc.items.push_back(BlockDescription{{{-1, -1}, {-0.1, 0}}});
            
            //Chute
            desc.items.push_back(SensorDescription{{{-0.1, -1}, {0.1, -0.7}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}});

            desc.items.push_back(PlayerDescription{{-0.8, 0.1}});
            desc.items.push_back(EscortDescription{{-0.8, 0.5}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.5, -1}, 1}} ,{}});

            return desc;
        }();
        return desc;
    }else if(index == 1){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            //Top wall
            desc.items.push_back(BlockDescription{{{-2, 0.9}, {2, 1}}});
            
            //Left wall
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}});
            
            //Top row!
            desc.items.push_back(BlockDescription{{{-1, -0.3}, {-0.185, 0.6}}});
            desc.items.push_back(BlockDescription{{{0.185, -1}, {0.7, 0.6}}});
            
            //Safe pocket
            desc.items.push_back(BlockDescription{{{0.7, -1}, {0.9, 0.1}}});
            desc.items.push_back(BlockDescription{{{0.9, -1}, {2, 1}}});
            desc.items.push_back(SensorDescription{{{0.7, 0.1}, {0.9, 0.35}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            
            //Bottom row
            desc.items.push_back(BlockDescription{{{-1, -1}, {-0.185, -0.7}}});
            
            //Chute
            desc.items.push_back(SensorDescription{{{-0.185, -1}, {0.185, -0.8}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}});
            
            desc.items.push_back(PlayerDescription{{-0.8, -0.6}});
            desc.items.push_back(EscortDescription{{-0.8, 0.8}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.5, -1}, 1}} ,{}});
            
            return desc;
        }();
        return desc;

    }else if(index == 2){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1.1, 1}}}); //1
            desc.items.push_back(BlockDescription{{{-1.1, 0.4}, {-0.7, 1}}}); //2
            desc.items.push_back(BlockDescription{{{-0.7, 0.8}, {-0.1, 1}}}); //3
            desc.items.push_back(BlockDescription{{{-0.1, 0.4}, {0.1, 1}}}); //4
            desc.items.push_back(BlockDescription{{{0.1, 0.6}, {2, 1.4}}}); //5
            desc.items.push_back(BlockDescription{{{-1.1, -1}, {-0.3, 0.2}}}); //6
            desc.items.push_back(BlockDescription{{{-0.3, -1}, {0.3, -0.2}}}); //7
            desc.items.push_back(BlockDescription{{{0.3, -1}, {0.4, 0}}}); //8
            desc.items.push_back(BlockDescription{{{0.4, -1}, {0.6, -0.5}}}); //9
            desc.items.push_back(BlockDescription{{{0.6, -1}, {1.4, -0.2}}}); //10
            desc.items.push_back(BlockDescription{{{0.3, 0}, {0.7, 0.2}}}); //11
            desc.items.push_back(BlockDescription{{{-0.19, -0.09}, {0.19, 0.2}}}); //12
            desc.items.push_back(BlockDescription{{{-0.1, 0.2}, {0.1, 0.4}}, true, false}); //13
            desc.items.push_back(BlockDescription{{{-0.5, 0.2}, {-0.3, 0.4}}, true, false}); //14
            desc.items.push_back(BlockDescription{{{-0.5, 0.4}, {-0.3, 0.6}}}); //15
            desc.items.push_back(BlockDescription{{{0.9, 0}, {1.4, 0.6}}}); //16

            desc.items.push_back(SensorDescription{{{1.3, -0.2}, {1.4, 0}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {}});
            desc.items.push_back(SensorDescription{{{1.1, -0.2}, {1.4, 0}}, {} ,{}, {SensorRenderers::BrokenLove{}}}); //Broken love

            desc.items.push_back(SensorDescription{{{0.4, -0.5}, {0.6, -0.25}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            
            //Death frame (out of range)
            desc.items.push_back(SensorDescription{{{-2, -1.3}, {2, -1.2}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {}});
            desc.items.push_back(SensorDescription{{{-2, 1.1}, {2, 1.3}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {}});
            desc.items.push_back(SensorDescription{{{1.5, -1}, {1.6, 1}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {}});
            desc.items.push_back(SensorDescription{{{-1.6, -1}, {-1.5, 1}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {}});

            desc.items.push_back(PlayerDescription{{-0.7, 0.3}});
            desc.items.push_back(EscortDescription{{-1, 0.3}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.44, -1}, 1}} ,{}});
            

            return desc;
        }();
        return desc;
    }else if(index == 3){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}}); //1
            desc.items.push_back(BlockDescription{{{-1, -0.5}, {-0.933, 0.1}}}); //2
            desc.items.push_back(BlockDescription{{{-0.933, -0.266}, {-0.533, -0.2}}}); //3
            desc.items.push_back(BlockDescription{{{-0.766, -0.5}, {-0.4, -0.4}}}); //4
            desc.items.push_back(BlockDescription{{{-0.766, -0.066}, {-0.4, 0.1}}}); //5
            desc.items.push_back(BlockDescription{{{-0.4, -1}, {-0.3, 0.1}}}); //6
            desc.items.push_back(BlockDescription{{{-1, -1}, {-0.4, -0.5}}}); //7
            //8 deleted
            desc.items.push_back(BlockDescription{{{-0.3, 0.4}, {-0.233, 0.5}}}); //9
            desc.items.push_back(BlockDescription{{{-0.066, 0.4}, {0, 0.5}}}); //10
            desc.items.push_back(BlockDescription{{{-1, 0.4}, {-0.7, 0.5}}}); //11
            desc.items.push_back(BlockDescription{{{0.5, -1}, {0.6, 0.1}}}); //11.2
            desc.items.push_back(BlockDescription{{{0.6, -1}, {0.8, -0.2}}}); //12
            desc.items.push_back(BlockDescription{{{0.8, -1}, {2, 1}}}); //13
            
            desc.items.push_back(BlockDescription{{{-2, 0.9}, {2, 1}}}); //roof

            desc.items.push_back(SensorDescription{{{-0.2, -1}, {0.5, -0.5}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{15}}}); //Broken love
            
            desc.items.push_back(SensorDescription{{{0.6, -0.2}, {0.8, 0.05}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            
            desc.items.push_back(PlayerDescription{{-0.7, 0.3}});
            desc.items.push_back(EscortDescription{{-0.9, 0.6}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.4, -1}, 1}} ,{}});
            
            
            
            
            desc.additionalActions = [](GameState& state){
                auto bridgeSwitch = createSwitch(state, {{-0.88, -0.5}, {-0.82, -0.42}}, {{-0.9, -0.5}, {-0.8, -0.48}});
                linkBlockToSwitch(state, bridgeSwitch, {{-0.3, 0}, {0.5, 0.1}});
            };
            return desc;
        }();
        return desc;
    }else if(index == 4){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}}); //0
            desc.items.push_back(BlockDescription{{{-1, -1}, {-0.3, 0.3}}}); //1
            desc.items.push_back(BlockDescription{{{-1, 0.5}, {-0.8, 0.6}}}); //2
            desc.items.push_back(BlockDescription{{{-0.3, -0.4}, {0, 0.3}}}); //3
            desc.items.push_back(BlockDescription{{{0, -0.4}, {0.5, -0.3}}}); //4
            desc.items.push_back(BlockDescription{{{0.5, -0.4}, {0.6, 0.3}}}); //5
            desc.items.push_back(BlockDescription{{{-0.5, 0.5}, {0, 0.6}}}); //6
            desc.items.push_back(BlockDescription{{{-1, 0.8}, {1, 1}}}); //7
            desc.items.push_back(BlockDescription{{{0.5, 0.5}, {0.8, 0.6}}}); //8
            desc.items.push_back(BlockDescription{{{-0.5, 0.3}, {-0.4, 0.5}}, true, false}); //9
            desc.items.push_back(BlockDescription{{{0.5, 0.3}, {0.6, 0.5}}, true, false}); //10
            desc.items.push_back(BlockDescription{{{0.8, -0.4}, {0.866, 0.6}}}); //11
            desc.items.push_back(BlockDescription{{{0.8, -1}, {1, -0.8}}}); //12
            desc.items.push_back(BlockDescription{{{1, -1}, {2, 1}}}); //13
            desc.items.push_back(BlockDescription{{{-0.1, -1}, {0.6, -0.7}}}); //14
            desc.items.push_back(BlockDescription{{{-0.3, -1}, {-0.1, -0.9}}}); //15

            
            desc.items.push_back(SensorDescription{{{0, -0.3}, {0.5, -0.1}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love
            desc.items.push_back(SensorDescription{{{0.6, -1}, {0.8, -0.9}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love

            desc.items.push_back(PlayerDescription{{-0.9, 0.4}});
            desc.items.push_back(EscortDescription{{-0.9, 0.7}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.4, -1}, 1}} ,{}});
            
            desc.items.push_back(SensorDescription{{{-0.3, -0.9}, {-0.1, -0.65}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});

            
            
            desc.additionalActions = [](GameState& state){
                auto bridgeSwitch1 = createSwitch(state, {{-0.18, 0.3}, {-0.12, 0.38}}, {{-0.2, 0.3}, {-0.1, 0.32}});
                linkBlockToSwitch(state, bridgeSwitch1, {{0, 0.5}, {0.5, 0.6}});
                auto bridgeSwitch2 = createSwitch(state, {{-0.18, 0.6}, {-0.12, 0.68}}, {{-0.2, 0.6}, {-0.1, 0.62}});
                linkBlockToSwitch(state, bridgeSwitch2, {{0, 0.2}, {0.5, 0.3}});
            };
            return desc;
        }();
        return desc;
    }else if(index == 5){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}}); //0
            desc.items.push_back(BlockDescription{{{-1, -1}, {-0.6, 0.2}}}); //1
            desc.items.push_back(BlockDescription{{{-0.6, -1}, {-0.4, -0.4}}}); //2
            desc.items.push_back(BlockDescription{{{-0.4, -1}, {-0.3, 0.2}}}); //3
            desc.items.push_back(BlockDescription{{{-0.3, -1}, {-0.1, -0.4}}}); //4
            desc.items.push_back(BlockDescription{{{-0.1, -1}, {0, 0.2}}}); //5
            desc.items.push_back(BlockDescription{{{0, -1}, {0.3, -0.4}}}); //6
            desc.items.push_back(BlockDescription{{{0.5, -1}, {1, -0.866}}}); //7
            desc.items.push_back(BlockDescription{{{-0.866, 0.5}, {-0.6, 0.7}}}); //8
            desc.items.push_back(BlockDescription{{{-0.866, 0.4}, {-0.8, 0.5}}}); //8'
            desc.items.push_back(BlockDescription{{{-0.6, 0.5}, {-0.4, 0.6}}}); //9
            desc.items.push_back(BlockDescription{{{-0.4, 0.5}, {-0.3, 0.7}}}); //10
            desc.items.push_back(BlockDescription{{{-0.3, 0.5}, {0.6, 0.6}}}); //11
            //12 deleted (it was just a jerk)
            desc.items.push_back(BlockDescription{{{0.2, 0.3}, {0.6, 0.5}}}); //13
            desc.items.push_back(BlockDescription{{{0.2, 0.1}, {0.3, 0.3}}}); //14
            desc.items.push_back(BlockDescription{{{0.5, -0.633}, {0.6, -0.1}}}); //15
            desc.items.push_back(BlockDescription{{{0.6, -0.733}, {0.8, -0.4}}}); //16
            desc.items.push_back(BlockDescription{{{0.8, -0.733}, {0.866, 0.1}}}); //17
            desc.items.push_back(BlockDescription{{{1, -1}, {2, 1}}}); //18
            desc.items.push_back(BlockDescription{{{-2, 0.9}, {2, 1}}}); //19
            desc.items.push_back(BlockDescription{{{0.5, -0.1}, {0.6, 0.3}}, true, false}); //20

            desc.items.push_back(SensorDescription{{{-0.5, -0.4}, {-0.3, -0.3}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love
            desc.items.push_back(SensorDescription{{{-0.2, -0.4}, {0, -0.3}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love
            desc.items.push_back(SensorDescription{{{0.3, -1}, {0.5, -0.9}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love

            desc.items.push_back(PlayerDescription{{-0.9, 0.8}});
            desc.items.push_back(EscortDescription{{-0.9, 0.3}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.27, -1}, 0.8}} ,{}});
            
            desc.items.push_back(SensorDescription{{{0.6, -0.4}, {0.8, -0.15}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            
            
            
            desc.additionalActions = [](GameState& state){
                auto bridgeSwitch1 = createSwitch(state, {{-0.53, 0.6}, {-0.47, 0.68}}, {{-0.55, 0.6}, {-0.45, 0.62}});
                linkBlockToSwitch(state, bridgeSwitch1, {{-0.6, 0.1}, {-0.4, 0.2}});
                auto bridgeSwitch2 = createSwitch(state, {{-0.23, 0.6}, {-0.17, 0.68}}, {{-0.25, 0.6}, {-0.15, 0.62}});
                linkBlockToSwitch(state, bridgeSwitch2, {{-0.3, 0.1}, {-0.1, 0.2}});
                auto bridgeSwitch3 = createSwitch(state, {{0.07, 0.6}, {0.13, 0.68}}, {{0.05, 0.6}, {0.15, 0.62}});
                linkBlockToSwitch(state, bridgeSwitch3, {{0, 0.1}, {0.2, 0.2}});
                auto switch4 = createSwitch(state, {{0.92, -0.866}, {0.98, -0.786}}, {{0.9, -0.866}, {1, -0.846}});
                linkBlockToSwitch(state, switch4, {{0.866, 0}, {1, 0.1}});
                switch4.on->updated.hook([bridgeSwitch1, bridgeSwitch2, bridgeSwitch3](bool on){
                    if(on){
                        bridgeSwitch1.on->setIfNotEqual(false);
                        bridgeSwitch2.on->setIfNotEqual(false);
                        bridgeSwitch3.on->setIfNotEqual(false);
                    }
                });
            };
            return desc;
        }();
        return desc;
    }else if(index == 6){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}}); //0
            desc.items.push_back(BlockDescription{{{-0.3, -1}, {-0.2, -0.566}}}); //1
            desc.items.push_back(BlockDescription{{{-1, -0.1}, {-0.466, 0}}}); //2
            desc.items.push_back(BlockDescription{{{-0.833, -0.4}, {-0.3, -0.3}}}); //3
            desc.items.push_back(BlockDescription{{{-0.3, -0.4}, {-0.2, 0}}}); //4
            desc.items.push_back(BlockDescription{{{-2, 0.933}, {2, 1}}}); //5
            desc.items.push_back(BlockDescription{{{-0.2, 0.3}, {0.5, 0.5}}}); //6
            desc.items.push_back(BlockDescription{{{0.8, 0.3}, {2, 0.933}}}); //7
            desc.items.push_back(BlockDescription{{{0.4, -1}, {2, 0}}}); //8
            desc.items.push_back(BlockDescription{{{0.1, -1}, {0.2, -0.3}}}); //9
            desc.items.push_back(BlockDescription{{{-0.2, -0.4}, {0.1, -0.3}}, true, false}); //10
            desc.items.push_back(BlockDescription{{{-1, 0.3}, {-0.2, 0.5}}, true, false}); //11
            desc.items.push_back(BlockDescription{{{-0.2, 0.166}, {-0.1, 0.3}}}); //12

            
            desc.items.push_back(SensorDescription{{{1.3, 0}, {1.4, 0.2}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {}});
            desc.items.push_back(SensorDescription{{{1.1, 0}, {1.4, 0.2}}, {} ,{}, {SensorRenderers::BrokenLove{}}}); //Broken love
            desc.items.push_back(SensorDescription{{{-0.2, -1}, {0.1, -0.8}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love
            desc.items.push_back(SensorDescription{{{0.2, -1}, {0.4, -0.8}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{}}}); //Broken love

            desc.items.push_back(PlayerDescription{{-0.8, -0.2}});
            desc.items.push_back(EscortDescription{{-0.8, 0.1}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.27, -1.5}, 0.5}} ,{}});
            
            desc.items.push_back(SensorDescription{{{-0.2, -0.7}, {0.1, -0.5}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            
            
            
            desc.additionalActions = [](GameState& state){
                auto bridgeSwitch1 = createSwitch(state, {{0.32, 0.5}, {0.38, 0.58}}, {{0.3, 0.5}, {0.4, 0.52}});
                linkBlockToSwitch(state, bridgeSwitch1, {{-0.2, -0.1}, {0.4, 0}});
                linkBlockToSwitch(state, bridgeSwitch1, {{-1, -.666}, {-0.3, -0.566}});
                auto bridgeSwitch2 = createSwitch(state, {{0.82, 0}, {0.88, 0.08}}, {{0.8, 0}, {0.9, 0.02}});
                bridgeSwitch2.on->set(true);
                bridgeSwitch2.on->updated.hook([bridgeSwitch1](bool on){
                    if(on) bridgeSwitch1.on->setIfNotEqual(false);
                });
                bridgeSwitch1.on->updated.hook([bridgeSwitch2](bool on){
                    if(on) bridgeSwitch2.on->setIfNotEqual(false);
                });

                auto bridgeSwitch3 = createSwitch(state, {{-.98, 0}, {-.92, 0.08}}, {{-1, 0}, {-0.9, 0.02}});
                linkBlockToSwitch(state, bridgeSwitch3, {{-0.2, -0.8}, {0.1, -0.7}});
                linkBlockToSwitch(state, bridgeSwitch3, {{-0.2, 0.5}, {-0.1, 0.933}});

            };
            return desc;
        }();
        return desc;
    }else if(index == 7){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1.3, 1}}}); //0
            desc.items.push_back(BlockDescription{{{-1.3, -0.3}, {-1.1, -0.1}}, true, false}); //1
            desc.items.push_back(BlockDescription{{{-1.1, -0.2}, {-1, 0.1}}, true, false}); //2
            desc.items.push_back(BlockDescription{{{-1, -0.3}, {-0.9, -0.1}}, true, false}); //3
            desc.items.push_back(BlockDescription{{{-0.9, -0.4}, {-0.3, -0.2}}, true, false}); //4
            desc.items.push_back(BlockDescription{{{-0.3, -0.3}, {0, 0.1}}, true, false}); //5
            desc.items.push_back(BlockDescription{{{0, -0.2}, {0.4, -0.1}}, true, false}); //6
            desc.items.push_back(BlockDescription{{{0.4, -0.5}, {0.5, 0}}, true, false}); //7
            desc.items.push_back(BlockDescription{{{0.5, -0.1}, {0.6, 0.3}}, true, false}); //8
            desc.items.push_back(BlockDescription{{{0.6, -0.2}, {0.9, -0.1}}, true, false}); //9a
            desc.items.push_back(BlockDescription{{{0.8, -1}, {0.9, -0.1}}}); //9b
            desc.items.push_back(BlockDescription{{{0.9, -1}, {1.1, -0.5}}}); //10
            desc.items.push_back(BlockDescription{{{1.1, -1}, {2, 1}}}); //11
            //12 absorbed into 11
            desc.items.push_back(BlockDescription{{{-0.6, 0.1}, {-0.5, 0.5}}, true, false}); //13
            desc.items.push_back(BlockDescription{{{0.2, 0.2}, {0.3, 0.5}}, true, false}); //14
            desc.items.push_back(BlockDescription{{{-2, 0.933}, {2, 1}}}); //ceiling
            
            desc.items.push_back(SensorDescription{{{-1.4, -1}, {1.4, -0.7}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{20}}}); //Broken love
            
            desc.items.push_back(PlayerDescription{{-1.2, 0}});
            desc.items.push_back(EscortDescription{{-1.2, 0.8}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.27, -1.5}, 0.5}} ,{}});
            
            desc.items.push_back(SensorDescription{{{0.9, -0.5}, {1.1, -0.2}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            
            
            desc.additionalActions = [](GameState& state){
                /*
                std::vector<SwitchData> switches;
                auto addSwitch = [&](Vector2<double> center){
                    switches.push_back(createSwitch(state, {center - Vector2<double>{0.03, 0}, center + Vector2<double>{0.03, 0.08}}, {center - Vector2<double>{0.05, 0}, center + Vector2<double>{0.05, 0.02}}));
                };
                addSwitch({-1.05, 0});
                addSwitch({-0.85, -0.2});
                addSwitch({-0.55, -0.2});
                addSwitch({-0.15, 0.1});
                addSwitch({0.25, -0.1});
                addSwitch({0.55, 0.1});
                linkBlockToSwitch(state, switches[0], {{0.9,-0.2}, {1.1, -0.1}});
                auto allOnLambda = copyToIndirectFunction([switches,&state](bool on){
                    if(on){
                        for(auto& sw : switches){
                            sw.on->setIfNotEqual(true);
                        }
                    }
                });
                for(auto& s : switches){
                    s.on->updated.hook(allOnLambda);
                }*/
            };
            return desc;
        }();
        return desc;
    }else if(index == 8){
        static LevelDescription desc = [](){
            LevelDescription desc;
            
            desc.items.push_back(BlockDescription{{{-2, -1}, {-1, 1}}}); //0
            desc.items.push_back(BlockDescription{{{-1, -1}, {-0.8, -0.4}}}); //1
            desc.items.push_back(BlockDescription{{{-0.8, -1}, {2, 0}}}); //2
            //desc.items.push_back(BlockDescription{{{-1, 0.35}, {2, 1}}, true, false}); //3

            
            desc.items.push_back(SensorDescription{{{1.15, 0}, {1.4, 1}}, {EscortActions::Kill{}} ,{PlayerActions::Kill{}}, {SensorRenderers::BrokenLove{10}}}); //Broken love
            
            desc.items.push_back(PlayerDescription{{1, 0.1}});
            desc.items.push_back(EscortDescription{{0.8, 0.6}});
            
            desc.items.push_back(SensorDescription{{{-5, -5}, {5, 5}}, {EscortActions::Propel{{0.27, -1.5}, 0.5}} ,{}});
            
            desc.items.push_back(SensorDescription{{{-1, -0.4}, {-0.8, -0.1}}, {EscortActions::Safety{}} , {PlayerActions::Safety{}}, {SensorRenderers::Love{}}});
            

            
            desc.additionalActions = [](GameState& state){
                
                 std::vector<SwitchData> switches;
                 auto addSwitch = [&](Vector2<double> center){
                 switches.push_back(createSwitch(state, {center - Vector2<double>{0.03, 0}, center + Vector2<double>{0.03, 0.08}}, {center - Vector2<double>{0.05, 0}, center + Vector2<double>{0.05, 0.02}}));
                 };
                 addSwitch({0.75, 0});
                 addSwitch({0.2, 0});
                 addSwitch({-0.15, 0});
                 linkBlockToSwitch(state, switches[0], {{-1,-0.1}, {-0.8, 0}});
                 auto allOnLambda = copyToIndirectFunction([switches,&state](bool on){
                     if(on){
                         for(auto& sw : switches){
                             sw.on->setIfNotEqual(true);
                         }
                     }
                 });
                 for(auto& s : switches){
                     s.on->updated.hook(allOnLambda);
                 }
            };
            return desc;
        }();
        return desc;
    }

    std::terminate();
}
