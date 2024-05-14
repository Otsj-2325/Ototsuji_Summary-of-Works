using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class SCR_PauseProc : MonoBehaviour
{
    [SerializeField] GameObject obj_Fade;
    [SerializeField] GameObject obj_Cursor;
    bool m_OldIsPless = false;

    // Start is called before the first frame update
    void Start()
    {
        obj_Fade.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (Gamepad.current == null) { return; }

        if (Gamepad.current.startButton.isPressed){
            if(m_OldIsPless){
                Debug.Log("Pressing start button."); 
            }
            else{
                bool isActive = obj_Fade.activeInHierarchy;
                obj_Fade.SetActive(!isActive);
                Debug.Log("NOT isActive");
            }
        }
        
        m_OldIsPless = Gamepad.current.startButton.isPressed;
    }
}
