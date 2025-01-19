using System;
using System.Collections.Generic;
using System.Text;

namespace EosScriptCore.Scripts
{
    public class SceneCamera : MonoBase
    {
        public static SceneCamera SceneController;
        private GameObject PlayerCamera;
        private Camera PlayerCameraCom;
        private GameObject Scene1;
        private Camera Scene1Cam;
        private GameObject TargetCamera;

        private GameObject Frame;
        private float moveSpeed;

        private GameObject BlackScreen;
        private UIComponent BlackScreenUI;

        public bool activateCutscene =  false;
        public bool blackin =  false;

        public float val = 0.0f;

        void OnStart()
        {
            SceneController = this;
            PlayerCamera = gameObject.FindWithTag("Camera");
            PlayerCameraCom = PlayerCamera.GetComponent<Camera>();
            Scene1 = gameObject.FindWithTag("SceneCamera1");
            Scene1Cam = Scene1.GetComponent<Camera>();
            TargetCamera = gameObject.FindWithTag("NTile26_08");
            Frame = gameObject.FindWithTag("KeyFrame");

        

            BlackScreen = gameObject.FindWithTag("BlackScreen");
            BlackScreenUI = BlackScreen.GetComponent<UIComponent>();
            

        }
        void OnUpdate()
        {
            if (activateCutscene)
            {
                CameraTransition();
            }
            else
            {

            }
          

        }

        public void ActivateCutscene()
        {
            activateCutscene = true;
            Scene1.SetActive(true);
            PlayerCamera.SetActive(false);
            BlackScreen.SetActive(true);
            BlackScreenUI.SetOpacity(0.0f);
        }

        public void DeactivateCutscene()
        {
            activateCutscene = false;
            PlayerCamera.SetActive(true);
            Scene1.SetActive(false);
            BlackScreen.SetActive(false);
        }


        public void CameraTransition()
        {

            Scene1Cam.SetCameraPosition(Scene1.transform.Translation);
            Scene1Cam.SetTargetPosition(TargetCamera.transform.Translation);
        }



        public void Blackout()
        {
            val += 0.01f;
            BlackScreenUI.SetOpacity(val);
            if (BlackScreenUI.GetOpacity() > 1.0f)
            {
                blackin = true;
            }
        }

        public void BlackIn()
        {
            val -= 0.01f;
            BlackScreenUI.SetOpacity(val);
            if (BlackScreenUI.GetOpacity() <= 0.0f)
            {
                
            }
            Log.LogInfo(val.ToString());
        }

        public void CameraTransitionFromData()
        {

        }
    }
}
