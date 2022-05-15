import IMediator from "./IMediator";

import CRUDLogic from "business logic/CRUDUseCases";
import passLogic from "business logic/passesUseCases";

export default class ConcreteMediators implements IMediator
{
    private passesCRUDManger: CRUDLogic;

    private satellitesCRUDManger: CRUDLogic;

    private passesManger: passLogic;

    constructor(passesCRUDManger: CRUDLogic, satellitesCRUDManger: CRUDLogic, passesManger: passLogic) 
    {
        this.passesCRUDManger = passesCRUDManger;
        this.passesCRUDManger.setMediator(this);

        this.satellitesCRUDManger = satellitesCRUDManger;
        this.satellitesCRUDManger.setMediator(this);

        this.passesManger = passesManger;
        this.passesManger.setMediator(this);
    }


    public async notify(sender: object, event: string)
    {
        return await this[event](sender);
    }

    private async newPassWasFount(passes)
    {
        const req = {body: passes};
        const res = {
            status: function() {
                return this;
            },
            json: function(data){
                if(data.data)
                    this.createdPasses = data.data;
            },
            createdPasses: []
        };
        await this.passesCRUDManger.create(req, res, () => {});
        return res.createdPasses;
    }

    private async getNewistPassTime(satId)
    {
        const req = {query: {'Satellite': satId, sort: { 'startTime' : -1 }}};
        const res = {
            status: function() {
                return this;
            },
            json: function(data){
                if(data.data)
                    this.newistPass = data.data;
            },
            newistPass: { startTime: undefined}
        };
        await this.passesCRUDManger.findOne(req, res, (e) => {throw new Error(e)});
        return res.newistPass.startTime;
    }

    private async getSatellite(satId)
    {
        if(!satId) throw new Error("please give a satellite id");

        const req = {params: {id: satId}};
        const res = {
            status: function() {
                return this;
            },
            json: function(data){
                if(data.data)
                    this.satellite = data.data;
            },
            satellite: {}
        };

        await this.satellitesCRUDManger.getSingleById(req, res, () => {});
        
        return res.satellite;
    }

    private async getAllSatellites(sendor)
    {
        const req = { query: {}};
        const res = {
            status: function() {
                return this;
            },
            json: function(data){
                if(data.data)
                    this.satellite = data.data;
            },
            satellite: []
        };

        await this.satellitesCRUDManger.getAll(req, res, (e) => {throw new Error(e);});
        return res.satellite;
    }

}