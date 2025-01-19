using System;
using System.Collections.Generic;
using System.Text;
using EosScriptCore;

namespace EosScriptCore.Scripts
{
    public class test1 : MonoBase
    {
        Vector3 pos = new Vector3(0.0f, 0.0f, 0.0f);
        GameObject[] tiles = new GameObject[10];

        public float var1 = 1234.1234f;

       public Transform comptest;
       public RigidBody body;
       public Camera cam;

        Vector3 newforce = new Vector3(5.0f, 0.0f, 0.0f);

        void OnStart()
        {
            comptest = gameObject.GetComponent<Transform>();
            body = gameObject.GetComponent<RigidBody>();
            cam = gameObject.GetComponent<Camera>();


           
        }

        void OnUpdate()
        {
            //float x = comptest.Translation.X;
            //float y = comptest.Translation.Y;
            //float z = comptest.Translation.Z;
            //comptest.Translation = new Vector3(x + 0.05f, y, z);

            //cam.SetMainCameraCenter(comptest.Translation);
            //cam.SetMainCameraEye(comptest.Translation);
            //cam.SetMainCameraUp(Vector3.Up);
            //
            //
            //Vector3 center = cam.GetMainCameraCenter();
            //Vector3 eye = cam.GetMainCameraEye();
            //Vector3 up = cam.GetMainCameraUp();

            //Log.LogInfo("Center: " + center.ToString());
            //Log.LogInfo("eye: " + eye.ToString());
            //Log.LogInfo("up: " + up.ToString());


            if(Input.IsKeyPressed(KeyCode.D))
            {
                for (int i = 0; i < 10; ++i)
                {
                    tiles[i] = Instantiate("red-tile", pos, Quaternion.identity);
                    Log.LogInfo(tiles.ToString());
                    pos += 1.0f;
                }
            }

            if (Input.IsKeyPressed(KeyCode.B))
            {
                for (int i = 0; i < 10; ++i)
                {
                    Destroy(tiles[i]); 
                }
            }



            //Log.LogInfo(body.entity.entity.ToString());


            //body.AddForce(newforce);
            body.SetAngularVelocity(newforce);
            body.SetRotate(newforce);
            

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
