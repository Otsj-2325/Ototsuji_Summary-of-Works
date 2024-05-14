using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object0307_DoorSwitch : MonoBehaviour
{
    [SerializeField] private List<Object0302_SwitchSystem> switchList = new List<Object0302_SwitchSystem>();
    [SerializeField] bool isSwitchOn = false;
    [SerializeField] bool getState = true;
    [SerializeField] bool isOpen = false;

    [SerializeField] Vector3 initPos = Vector3.zero;
    [SerializeField] protected float upLength = 0.0f;
    [SerializeField] float moveFrame = 0.0f;
    [SerializeField] float frameCnt = 0.0f;

    private void Start()
    {
        initPos = transform.position;
    }

    private void Update()
    {
        if (Time.timeScale == 0) return; //ヒットストップ中

        GetState();
        MoveProc();
    }

    public void GetState()
    {
        frameCnt++;

        if (getState)
        {
            foreach (Object0302_SwitchSystem switches in switchList)
            {
                if (!switches.isOnSwitch)
                {
                    isSwitchOn = false;
                    break;
                }

                isSwitchOn = true;
            }

            if (isSwitchOn && !isOpen)
            {
                initPos = transform.position;
                frameCnt = 0;
                getState = false;
            }
            else if(!isSwitchOn && isOpen)
            {
                frameCnt = 0;
                getState = false;
            }
        }

    }

    private void MoveProc()
    {
        var pos = transform.position;

        if (isSwitchOn)
        {
            if (transform.position.y >= initPos.y + upLength)
            {
                pos.y = initPos.y + upLength;
                getState = true;
                isOpen = true;
            }
            else
            {
                pos.y = initPos.y + upLength * (frameCnt / moveFrame);
            }
        }
        else
        {
            if (transform.position.y <= initPos.y)
            {
                pos.y = initPos.y;
                getState = true;
                isOpen = false;
            }
            else
            {
                pos.y = pos.y - upLength * (1 / moveFrame);
            }
        }

        transform.position = pos;
    }
}
