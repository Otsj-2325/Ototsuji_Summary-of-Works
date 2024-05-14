using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object0302_SwitchSystem : MonoBehaviour
{
    [SerializeField] GameObject top;
    [SerializeField] protected float downLength = 0.0f;
    [SerializeField] float moveFrame = 0.0f;
    [SerializeField] protected List<string> ctrlObjectsTag;
    float frameCnt = 0.0f;

    private SpriteRenderer sRenderer;

    public bool isOn = false;
    public bool isOnSwitch;
    public bool isMoveTop = false;

    [SerializeField] Sprite onSprite = null;
    [SerializeField] Sprite offSprite = null;

    protected Collider2D col2d;

    [SerializeField] bool getState = false;

    // Start is called before the first frame update
    protected virtual void Start()
    {
        if (top)
        {
            sRenderer = top.GetComponent<SpriteRenderer>();
        }
    }
    
    protected void GetStateProc()
    {
        if (!top || getState)
        {
            if (isOn != isOnSwitch)
            {
                frameCnt = 0;
                getState = false;

                if (sRenderer)
                {
                    if (isOnSwitch)
                    {
                        sRenderer.sprite = onSprite;
                    }
                    else
                    {
                        sRenderer.sprite = offSprite;
                    }
                }
            }


            if (isOnSwitch)
            {
                isOn = true;
            }
            else
            {
                isOn = false;
            }
        }
    }

    protected void MoveTopProc()
    {
        if (!top)
        {
            return;
        }

        var pos = top.transform.localPosition;
        var offset = col2d.offset;

        if (isMoveTop)
        {
            if (isOnSwitch)
            {
                if (top.transform.localPosition.y <= -downLength)
                {
                    pos.y = -downLength;
                    offset.y = -downLength;
                    getState = true;
                    isMoveTop = false;
                }
                else
                {
                    pos.y = -downLength * (frameCnt / moveFrame);
                    offset.y = -downLength * (frameCnt / moveFrame);
                    isMoveTop = true;
                }
            }
            else
            {
                if (top.transform.localPosition.y >= 0)
                {
                    pos.y = 0.0f;
                    offset.y = 0.0f;
                    getState = true;
                    isMoveTop=false;

                }
                else
                {
                    pos.y += downLength * (1 / moveFrame);
                    offset.y += downLength * (1 / moveFrame);
                    isMoveTop = true;
                }
            }
        }

        top.transform.localPosition = pos;
        col2d.offset = offset;

        frameCnt++;
        if (frameCnt > moveFrame)
        {
            frameCnt = moveFrame;
        }
    }
}