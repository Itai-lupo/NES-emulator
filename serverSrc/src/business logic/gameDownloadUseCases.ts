import ErrorResponse from "../utils/errorResponse";
import path from "path"

import {
    returnSuccessRespondToTheClient,
    returnRespondToTheClientWithErr,
} from '../utils/sendResponse';

import globby from "globby"

export default class gameDownloadLogic
{

    downloadGame = async (req, res, next) => {
        res.set("gameName", req.params.gameName);
        res.sendFile( path.join(__dirname, '../../res/games/') + req.params.gameName + ".nes")
    }
    
    gameList = async (req, res, next) => {
        let paths = await globby(path.join(__dirname, "../../res/games/*.nes"));  
        paths = paths.map((c) => {  return (c.split("/").pop()).split(".")[0] })
        returnSuccessRespondToTheClient(res, 200, paths)
    }

}