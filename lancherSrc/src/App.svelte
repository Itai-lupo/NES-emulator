<script>
	import axios from "axios"
	import LoginCard from './components/loginCard.svelte' 
	import ServersList from './components/serversList.svelte' 
	import GameList from './components/gamesList.svelte' 

	import { getGameList } from './renderer.js';
	
	

    let gameList = getGameList();

	let servers = [];

	function addServer() {
		showAddServer = true;
	}

	async function  addServerD(e) {
		showAddServer = false;

		try {
			const res = await axios.post(e.detail.host + '/api/v1/login', e.detail);
			
			
			console.log("added " + e.detail.host + '/api/v1/login')

			const gameListRes = await axios.get(e.detail.host + '/api/v1/getGameList');

			servers = servers.concat(
			{
				...e.detail,
				token: res.data.data,
				games: gameListRes.data.data
			})

			console.log(servers)

		} catch (error) {
			if(error.response)
				alert(error.message + " " + error.response.data.error)
			else		
				alert(error.message)
		}
	}


	let showAddServer = false;
</script>

<dev class="main" class:blur = "{showAddServer}">
	<h1> NES lancher </h1>
	<button class="addServer" on:click={addServer}>add server</button>
	
	<ServersList {servers} bind:gameList = {gameList}/>
	<GameList bind:gameList = {gameList} />
</dev>

<dev class = "floatCard" class:visibily="{showAddServer}">
	<LoginCard on:submit = {addServerD} bind:show = {showAddServer} />
</dev>

<style>
	.floatCard{
		position: absolute;
		top: 25%;
		right: 25%;
		width: 50%;
		height: 50%;
		visibility: hidden;	
	}
	
	.visibily{
		visibility: visible;
	}

	.blur > *
	{
		/* filter: blur(3px); */
	}

	.addServer
	{
		display: block;
	}
</style>