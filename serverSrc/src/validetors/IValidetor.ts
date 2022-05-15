export default abstract class IValidetor
{
    validateUpdate(dataToValidate){
        return { id: dataToValidate.params.id, dataToUpdate: dataToValidate.body};
    }

    validateCreate(dataToValidate: any) {        
        return dataToValidate.body;
    }
    validateDelete(dataToValidate: any) {
        return dataToValidate.params.id;
    }
    validateGetAll(dataToValidate: any) {
        return dataToValidate.query || {};
    }
    validateGetById(dataToValidate: any) {
        return { id: dataToValidate.params.id, query: dataToValidate.query};
    }
    validateFindOne(dataToValidate: any) {
        return dataToValidate.query || {};
    }
}