using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SCR_Goal : MonoBehaviour
{
    [SerializeField] GameObject obj_Fade;
    SCR_FadeProccess cm_Fade;
    [SerializeField] string m_LoadScene;

    // Start is called before the first frame update
    void Start()
    {
        cm_Fade = obj_Fade.GetComponent<SCR_FadeProccess>();
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Player")
        {
            Debug.Log("Goal");
            obj_Fade.gameObject.SetActive(true);
            cm_Fade.StartFadeOut(m_LoadScene);
        }
    }
}
