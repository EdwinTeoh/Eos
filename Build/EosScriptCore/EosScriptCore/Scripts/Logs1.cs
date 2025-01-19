using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    class Logs1 : MonoBase
    {
        private bool displayLogs = false;
        private GameObject player;
        private GameObject logsImage;

        void OnStart()
        {
            player = gameObject.FindWithTag("Surge");
            logsImage = gameObject.FindWithTag("Logs1");
            logsImage.SetActive(false);
        }

        void OnUpdate()
        {
            if (displayLogs == true)
            {
                logsImage.SetActive(true);
            }
            else
            {
                logsImage.SetActive(false);
            }
        }

        void OnTriggerBegin(ulong entity)
        {
            if (player.GetComponent<Collider>().isTrigger)
            {
                displayLogs = true;
            }
        }

        void OnTriggerEnd(ulong entity)
        {
            if (player.name == "Surge")
            {
                displayLogs = false;
            }
        }
    }
}
