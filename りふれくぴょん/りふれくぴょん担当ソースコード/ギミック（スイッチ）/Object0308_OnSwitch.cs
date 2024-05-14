using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;
using UnityEngine;

[RequireComponent(typeof(BoxCollider2D))]

public class Object0308_OnSwitch : Object0302_SwitchSystem
{
    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
        col2d = GetComponent<BoxCollider2D>();

    }

    //Update is called once per frame
    protected override void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        GetStateProc();
        MoveTopProc();
    }

    protected void OnTriggerEnter2D(Collider2D collision)
    {
        foreach (string s in ctrlObjectsTag)
        {
            if (collision.gameObject.tag == s)
            {
                if (!isMoveTop || !isOnSwitch)
                {
                    isOnSwitch = true;
                    isMoveTop = true;
                }
            }

        }
    }
}