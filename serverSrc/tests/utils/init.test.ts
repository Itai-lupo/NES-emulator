require("../../src/utils/dotenvInit");

import {createAPIManger, createDBManger} from "../../src/utils/MangersInit";

import expressApi from "../../src/IO_Mangers/ApiManger/expressApi";

import AuthDBManger from "../../src/IO_Mangers/DBManger/mongoDB/AuthDBManger";


test("api init", () => {
    const output = createAPIManger(6000);

    expect(output instanceof  expressApi).toBe(true);
    output.close();
})


test("db init", () => {
    const {usersDBManger} = createDBManger();

    expect(usersDBManger instanceof AuthDBManger).toBe(true);

})
