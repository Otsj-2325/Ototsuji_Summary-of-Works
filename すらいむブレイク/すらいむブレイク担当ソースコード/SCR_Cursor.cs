using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.InputSystem;
using UnityEngine.UI;

public class SCR_Cursor : MonoBehaviour
{
    [SerializeField] List<RectTransform> m_PositionList;
    [SerializeField] List<UnityEvent> m_ButtonEventList;
    int m_PosIndex = 0;

    // Start is called before the first frame update
    void Start()
    {
        transform.position = m_PositionList[0].position;
    }

    // Update is called once per frame
    void Update()
    {
        if (Gamepad.current == null) { return; }

        transform.position = m_PositionList[m_PosIndex].position;

        if (Gamepad.current.leftStick.ReadValue().y > 0)
        {
            m_PosIndex--;

            if(m_PosIndex < 0){
                m_PosIndex = 0;
            }
        }
        else if (Gamepad.current.leftStick.ReadValue().y < 0)
        {
            m_PosIndex++;

            if(m_PosIndex > m_PositionList.Count -1){
                m_PosIndex = m_PositionList.Count - 1;
            }
        }

        if(Gamepad.current.buttonSouth.isPressed){
            m_ButtonEventList[m_PosIndex].GetPersistentTarget(0).GetComponent<Button>().onClick.Invoke();
        }
    }

}
