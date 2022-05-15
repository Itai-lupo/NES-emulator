import { ObjectId } from 'mongoose';

export default interface IDBManager
{
	connect(URI): void;

    getAll(query?, params?);
    getSingleById(id, params?);
    findOne(query, params?);
    create(UserToCreate);
    update(id, dataToUpdate);
	delete(id);
	
    getAmount();
}
