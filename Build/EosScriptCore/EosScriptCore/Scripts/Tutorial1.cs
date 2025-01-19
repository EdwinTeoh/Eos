using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Tutorial1 : MonoBase
    {
        private bool displayTutorial = false;
        private GameObject player;
        private GameObject tutorialImage;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            tutorialImage = gameObject.FindWithTag("TutorialImage1");
            tutorialImage.SetActive(false);
        }

        void OnUpdate()
        {
            if (displayTutorial == true)
            {
                tutorialImage.SetActive(true);
            }
            else
            {
                tutorialImage.SetActive(false);
            }
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                displayTutorial = true;
                SceneCamera.SceneController.ActivateCutscene();
            }
        }

        void OnTriggerEnd(ulong entity)
        {
            if (!player.GetComponent<Collider>().isTrigger)
            {
                displayTutorial = false;
                SceneCamera.SceneController.DeactivateCutscene();
            }
        }
    }
}
