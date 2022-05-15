import IAPIManager from '../src/IO_Mangers/ApiManger/IAPIManager';
import expressApi from '../src/IO_Mangers/ApiManger/expressApi';
const request = require('supertest');


let api:IAPIManager;

beforeEach(() => {
    api = new expressApi(5555);
})

afterEach( () => {
    api.close();
})

describe("check route funcnalty", () => {
    test("add get request at route /test1", () => {
        const  test = () =>{ return true}
        api.addRoute("get", "/test1", test );

        api.getApp()._router.stack.forEach( r =>{
            if (r.route && r.route.path)
            {
              expect(r.route.path).toBe("/test1");
              expect(r.route.methods.get).toBe(true);        
              expect(r.route.stack[0].name).toBe("test");
              return undefined;
            }
          })
    })

    test("add get request at route /test2 and send to it", (done) => {
        const test = (req, res) => {
                res.send(200, { sucssus: true });
            }
        api.addRoute("get", "/test1", test );

        request(api.getApp())
            .get('/test1')
            .expect(200)
            .expect(JSON.stringify({ sucssus: true }))
            .then(v => done());
    })
})




