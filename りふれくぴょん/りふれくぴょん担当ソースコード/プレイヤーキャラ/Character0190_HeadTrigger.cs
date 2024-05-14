using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(BoxCollider2D))]

public class Character0190_HeadTrigger : MonoBehaviour
{
    public bool isCheck = false;

    // Start is called before the first frame update
    void Start()
    {
        this.gameObject.GetComponent<BoxCollider2D>().isTrigger = true;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Ground" || collision.gameObject.tag == "Ceiling" || collision.gameObject.tag == "Block" || collision.gameObject.tag == "Wall")
        {
            isCheck = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Ground" || collision.gameObject.tag == "Ceiling" || collision.gameObject.tag == "Block" || collision.gameObject.tag == "Wall")
        {
            isCheck = false;
        }
    }
}
