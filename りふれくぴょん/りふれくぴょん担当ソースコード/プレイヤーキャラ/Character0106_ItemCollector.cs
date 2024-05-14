/**************************************************
 * update : 2022/ 03/ 22
 * written : otsj.
 * about : 
 * プレイキャラアイテム収集処理
***************************************************/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(BoxCollider2D))]

public class Character0106_ItemCollector : MonoBehaviour
{
    [SerializeField] int gPoint;
    [SerializeField] int sPoint;
    [SerializeField] int bPoint;

    System0901_NumDisplay numDisp;

    public int gCnt = 0;
    public int sCnt = 0;
    private int bCnt = 0;

    // Start is called before the first frame update
    void Start()
    {
        numDisp = FindObjectOfType<System0901_NumDisplay>();  
    }
    // Start is called before the first frame update

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Coin_G")
        {
            // 五十嵐　スターコイン獲得SE
            Sound.instance.PlaySE(13);
            collision.gameObject.SetActive(false);
            Destroy(collision.gameObject);
            var cCnt = PlayerPrefs.GetInt(System0101_GameManager.instance.CollectCoinCounter);
            cCnt += gPoint;

            ++gCnt;
            PlayerPrefs.SetInt(System0101_GameManager.instance.CollectCoinCounter, cCnt);
            Debug.Log("coin collected");
             numDisp.display = cCnt;

        }
        if (collision.gameObject.tag == "Coin_S")
        {
            // 五十嵐　コイン獲得SE
            Sound.instance.PlaySE(12);
            collision.gameObject.SetActive(false);
            Destroy(collision.gameObject);
            sCnt += sPoint;
        }
        if (collision.gameObject.tag == "Coin_B")
        {
            collision.gameObject.SetActive(false);
            Destroy(collision.gameObject);
            sCnt += sPoint;

        }
    }


    public int GetGCnt()
    {
        return gCnt;
    }
}
