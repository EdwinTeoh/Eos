using System;
using System.Collections.Generic;
using System.Text;



namespace EosScriptCore.Scripts
{
    public class test2 : MonoBase
    {
        void OnStart()
        {
            Log.LogInfo("NEW SCRIPT init");
        }

        void OnUpdate()
        {

            Log.LogInfo("NEW SCRIPT runnning");
            if(Input.IsKeyPressed(KeyCode.P))
            {
                GameStateManager.SetGameState(GameStateManager.ENGINESTATE.STOP);
                SceneMananger.LoadNextScene("Assets/Data/Level_1.json");
            }

            if (Input.IsKeyPressed(KeyCode.K))
            {
               // GameStateManager.SetGameState(GameStateManager.ENGINESTATE.PLAY);
            }





        }

        void OnFixedUpdate()
        {

        }

        void OnTriggerBegin(ulong data)
        {

        }

        void OnTriggerStay(ulong data)
        {

        }

        void OnTriggerEnd(ulong data)
        {

        }

    }
}
