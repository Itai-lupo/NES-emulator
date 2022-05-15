import mongoose from "mongoose";

import { ObjectId } from 'mongoose';

import IDBManager from "../intrface/IDBManger";



export default abstract class mangoDBManger implements IDBManager
{
    connect(URI) 
    {
        mongoose.connect(URI, {
            useNewUrlParser: true,
            useCreateIndex: true,
            useFindAndModify: false,
            useUnifiedTopology: true,
            keepAlive: true
        })
        .then(conn => console.log(`mongoDB Connected: ${conn.connection.host}`));
    }   

    protected formatTheDBRequst(dbRequst: any, params: any, populate:any = '') {
        if(Object.keys(params).length === 0) return dbRequst;
        
        if (this.populateRequset(params, populate))
            dbRequst = dbRequst.populate(populate);
            
        dbRequst = dbRequst.select(params.select);
        dbRequst = dbRequst.sort(params.sort);
        dbRequst = dbRequst.skip(params.skip);
        dbRequst = dbRequst.limit(params.limit);
        return dbRequst;
    }

    private populateRequset(params: any, populate: any) {
        if(!params.select)  
            return true;
            
        return params.select == "" ||
            (params.select.includes(populate) && !params.select.includes(`-${populate}`));
    }


    abstract getAll(query?, params?);
    abstract getSingleById(id:ObjectId, params?);
    abstract findOne(query, params?);
    abstract create(UserToCreate);
    abstract update(id:ObjectId, dataToUpdate);
	abstract delete(id:ObjectId);
	
    abstract getAmount();
}