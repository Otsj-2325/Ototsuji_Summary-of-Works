using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(BoxCollider2D))]

public class Object0301_TactSwitch : Object0302_SwitchSystem
{
    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
        col2d = GetComponent<BoxCollider2D>();

    }

    // Update is called once per frame
    protected override void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        GetStateProc();
        MoveTopProc();
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        foreach (string s in ctrlObjectsTag)
        {
            if (collision.gameObject.tag == s)
            {
                if (!isMoveTop)
                {
                    isOnSwitch = true;
                    isMoveTop = true;
                }
            }

        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {

        foreach (string s in ctrlObjectsTag)
        {
            if (collision.gameObject.tag == s)
            {
                {
                    isOnSwitch = false;
                    isMoveTop = true;
                }

            }
        }

    }
}