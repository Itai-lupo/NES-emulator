const { remote } = require('electron');
const ipc = require('electron').ipcRenderer;

export function getGameList() 
{
    
    const res = ipc.sendSync('getGameList');
    console.log(res)
    return res;
}

export function lanchGame(gameName)
{
    const res = ipc.sendSync('lanchGame', gameName);
    console.log(res)


}

export function downloadGame(gameName, server)
{
    const res = ipc.sendSync('downloadGame', {gameName, server});
    console.log(res)

}