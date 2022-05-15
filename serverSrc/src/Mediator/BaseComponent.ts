import IMediator from "./IMediator";

export default abstract class BaseComponent {
    protected mediator: IMediator;

    constructor(mediator: IMediator = null) {
        this.mediator = mediator;
    }

    public setMediator(mediator: IMediator): void {
        this.mediator = mediator;
    }
}