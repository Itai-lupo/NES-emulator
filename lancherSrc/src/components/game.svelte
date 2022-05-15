<script>
    import { lanchGame } from '../renderer.js'

    export let game;
  
    function startGame()
    {
        console.log(game)
        lanchGame(game)
    }
</script>

<div>
    <button type="button" class="collapsible"  on:click={startGame}> {game} </button>
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
    .collapsible:hover {
        background-color: #6A6A6A;
    }
    
</style>