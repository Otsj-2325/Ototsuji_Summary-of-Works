using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SCR_Knife : MonoBehaviour
{
    [SerializeField] Vector3 speed;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        var rot = transform.eulerAngles;
        rot.x += speed.x;
        rot.y += speed.y;
        rot.z += speed.z;
        transform.eulerAngles = rot;
    }
}
