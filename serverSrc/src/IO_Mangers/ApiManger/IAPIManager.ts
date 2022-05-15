export default interface IAPIManager 
{
	close(): void;
	getApp(): any;
    initMiddleware(): void;


	addRoute(method:string, path: string, functionToCall: any);
}
