using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;

public class SCR_HotToMenu : MonoBehaviour
{
    // Start is called before the first frame update
    void Awake()
    {
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (Gamepad.current == null) { return; }

        if (Gamepad.current.buttonEast.isPressed){
            gameObject.SetActive(false);
        }
    }

    public void ActiveProc(){
        gameObject.SetActive(true);
    }
}
