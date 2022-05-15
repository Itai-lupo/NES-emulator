<script>

    export let gameList;
    import Game from "./game.svelte";
</script>

<dev>
    <h2>
        <span class="missing">downloaded games list</span>
    </h2>

    {#each gameList as game}
        <Game {game} />
    {/each}
</dev>


<style>
    dev{
        display: inline-block;
        margin: 1em;
        width: 40%;
        height: 85%;
        box-sizing: border-box;
        border: 1px solid black;

        position: absolute;
        left: 50%;
        top: 130px;
        
        overflow: auto;
    }


    h2 {
        padding: 0 0 0.2em 0;
        margin: 1em 0 0 0;
        border-bottom: 1px solid #ff3e00;
        text-align: center;
    }
</style>