using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class SCR_CameraCtrl : MonoBehaviour
{
    [SerializeField] float m_Offset;
    [SerializeField] GameObject obj_Player;
    [SerializeField] float m_CamSpeed;
    [SerializeField] float m_DeadZone;

    Vector3 m_CalPos;
    Vector2 m_CamRot;
    Camera m_camera;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;

        if(obj_Player){
            m_CalPos = transform.position;            
        }

        m_camera = GetComponent<Camera>();

        m_CamRot = new Vector2(-1.0f, 0.0f);
    }

    // Update is called once per frame
    void Update()
    {
        if (Gamepad.current == null) { return; }

        m_CalPos = transform.position;

        var stick = Gamepad.current.rightStick;
        
        if(stick.ReadValue().x > m_DeadZone){
            m_CamRot.x += m_CamSpeed;
        }
        else if(stick.ReadValue().x < -m_DeadZone){
            m_CamRot.x -= m_CamSpeed;
        }

        if (stick.ReadValue().y > m_DeadZone)
        {
            m_CalPos.y += m_CamSpeed;
        }
        else if (stick.ReadValue().y < -m_DeadZone)
        {
            m_CalPos.y -= m_CamSpeed;
        }

        if(m_CalPos.y - obj_Player.transform.position.y > m_Offset){
            m_CalPos.y = obj_Player.transform.position.y + m_Offset;
        }
        
        if (m_CalPos.y < obj_Player.transform.position.y)
        {
            m_CalPos.y = obj_Player.transform.position.y;
        }

        var valCos = Mathf.Cos(m_CamRot.x);
        var valSin = Mathf.Sin(m_CamRot.x);

        var plrPos = obj_Player.transform.position;

        m_CalPos.x = m_Offset * valCos + plrPos.x;
        m_CalPos.z = m_Offset * valSin + plrPos.z;

        transform.position = m_CalPos;

        transform.LookAt(obj_Player.transform.position);
    }
    
    public Vector3 GetTargetVector(){
        return obj_Player.transform.position - transform.position;
    }
}
