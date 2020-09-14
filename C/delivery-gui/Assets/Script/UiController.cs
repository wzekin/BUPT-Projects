using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UiController : MonoBehaviour
{
    //绑定退出按钮
    public void Quit()
    {
        Debug.Log("exit");
        Application.Quit();
    }
}
