<script>
    export let server;
    export let gameList;

    import { downloadGame } from '../renderer.js'

    let isCollapsed = true;

    function toggle()
    {
        isCollapsed = !isCollapsed;
    }

    function collapse(node, { duration }) {
		return {
			duration,
			css: t => {
				const eased = elasticOut(t);

				return `
					max-height: eased`
			}
		};
	}

    function accordion(node, isOpen) {
		let initialHeight = node.offsetHeight;
		node.style.height = isOpen ? 'auto' : 0;
		node.style.overflow = "hidden";
		return {
			update(isOpen) {
				let animation = node.animate(
					[
						{
							height: initialHeight + 'px',
							overflow: 'hidden'
						},
						{
							height: 0,
							overflow: 'hidden'
						}
					],
					{ duration: 100, fill: 'both' }
				);
				animation.pause();
				if (!isOpen) {
					animation.play();
				} else {
					animation.reverse();
				}
			}
		};
	}

    const getGame = (gameName) => () =>
    {
        if(gameList.includes(gameName))
            return;
        gameList = gameList.concat(gameName)
        downloadGame(gameName, server)
    }

</script>

<div>
    <button type="button" class="collapsible" class:active = "{!isCollapsed}" on:click={toggle}> {server.host} </button>
    <!-- {#if !isCollapsed} -->
        <div class="content" class:contentActive="{!isCollapsed}" use:accordion={!isCollapsed}>
            {#each server.games as game }
                <button type="button" class="game" on:click={getGame(game)} name = {game}>{game}</button>
            {/each}
        </div>
    <!-- {/if} -->
</div>


<style>
    div{
        display: block;
        border-bottom: 1px solid black;
    }

    .collapsible {
        background-color: #777;
        color: black;
        cursor: pointer;
        padding: 18px;
        margin: 0;
        width: 100%;
        border: none;
        text-align: left;
        outline: none;
        font-size: 15px;
    }

    /* Add a background color to the button if it is clicked on (add the .active class with JS), and when you move the mouse over it (hover) */
    .active, .collapsible:hover {
        background-color: #6A6A6A;
    }
    
    .content {
        display: block;
        padding: 0 18px;
        background-color: #919191;
        color: black;
        overflow: hidden;
    }

    .collapsible:after {
        content: '\02795'; /* Unicode character for "plus" sign (+) */
        font-size: 13px;
        color: white;
        float: right;
        margin-left: 5px;
    }

    .active:after {
        content: "\2796"; /* Unicode character for "minus" sign (-) */
    }

    .game
    {
        margin: 0.1em;
    }
</style>