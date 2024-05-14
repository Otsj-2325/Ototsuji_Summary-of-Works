using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object0304_UpperSwitch : Object0302_SwitchSystem
{
    [SerializeField] List<GameObject> keyObjects;

    // Start is called before the first frame update
     protected override void Start()
    {
        base.Start();
    }

    //// Update is called once per frame
    protected override void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        foreach (GameObject g in keyObjects)
        {
            if(g == null){
                isOnSwitch = false;
                break;
            }

            if (!g.activeInHierarchy)
            {
                isOnSwitch = false;
                break;
            }

            isOnSwitch = true;
        }

        GetStateProc();
    }
}
