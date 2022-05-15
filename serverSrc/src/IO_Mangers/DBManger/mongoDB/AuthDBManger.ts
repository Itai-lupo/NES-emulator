import mangoDBManger from './mangoDBManger'

import User from "../models/User";

export default class AuthDBManger extends mangoDBManger
{
    protected userAmount: number;

    async findOne(query: any, params: any = {}) {
        const user = await User.findOne(query, {}, params);
        return user;
    }
    
    async getAll(query:any = {}, params:any = {}) {
        let dbRequst = User.find(query);
        
        this.formatTheDBRequst(dbRequst, params, '');

        return await dbRequst;
    }
    
    async getSingleById(id: any, params:any = { options: {}, match: {}}) {
        const resUser = await User.findById(id).populate([{
                path: 'planedPasses',
                match : { ...params.match.planedPasses},
                options: { ...params.options.planedPasses},
                populate: "Satellite"
            },{
                path: 'operatedPasses',
                match : { ...params.match.operatedPasses},
                options: { ...params.options.operatedPasses},
                populate: "Satellite"
            }]);
            
        return resUser;
    }

    async create(UserToCreate: any) {
        const newUser = await User.create(UserToCreate);
        return newUser;
    }
    
    async update(id: any, dataToUpdate: any) {
        const updatedUser = await User.findByIdAndUpdate(id, dataToUpdate, {new: true});

        return updatedUser;
    }
    
    async delete(id: any) {
        const deleted = await User.findByIdAndDelete(id);
        return deleted;
    }
    
    getAmount() 
    {
        User.countDocuments({}, (err, count) => {
            this.userAmount = count;
        })

        return this.userAmount;
    }
}