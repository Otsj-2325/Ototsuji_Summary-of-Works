using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object0305_DownerSwitch : Object0302_SwitchSystem
{
    [SerializeField] private List<GameObject> keyObjects;

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
            if(g == null)
            {
                isOnSwitch = true;
                continue;
            }

            if (g.activeInHierarchy){
                isOnSwitch = false;
                break;
            }

            isOnSwitch = true;
        }

        GetStateProc();
    }
}
