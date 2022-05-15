<script>
    import ServerItem from "./server.svelte"

    export let servers;
    export let gameList;
</script>

<dev>
    <h2>
        <span class="missing">servers list</span>
    </h2>

    {#each servers as server}
        <ServerItem {server} bind:gameList = {gameList}/>
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
        left: 15px;
        top: 130px;        
    }

    h2 {
        padding: 0 0 0.2em 0;
        margin: 1em 0 0 0;
        border-bottom: 1px solid #ff3e00;
        text-align: center;
    }
</style>