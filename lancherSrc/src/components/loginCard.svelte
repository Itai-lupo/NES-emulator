<script>
	import { createEventDispatcher } from 'svelte';
    const dispatch = createEventDispatcher();

    export let show;

    let user = {
        host: "http://localhost:8081",
        email: "itailupo@gmail.com",
        password: "Ii8940706"
    };


    let error = {
    };

    function onSubmit(e) {
        const formData = new FormData(e.target);

        const data = {};
        for (let field of formData) {
            const [key, value] = field;
            data[key] = value;
        }

        let valid = true;
        
        if(!data.host){
            error.host = true;
            valid = false;
        }


        if(!data.email){
            error.email = true;
            valid = false;
        }


        if(!data.password){
            error.password = true;
            valid = false;
        }

        if(valid)
        {
            error = {};
            user.host = "";
            user.port = 0;
            dispatch('submit', data);
        }
    }
</script>


<article class="contact-card">
    <a href="#" class="exit" on:click={() => show = false}/>

    <h2>
        <span class="missing">server details </span>
    </h2>


    <form on:submit|preventDefault={onSubmit}>

        <div class="host">
            <label for="name">server host</label>
            <input type="text" class="missing" name="host" bind:value={user.host} />

            {#if error.host}
                <p class="error-message">server host is required</p>
            {/if}
        </div>

        <div class="email">
            <label for="name">Email</label>
            <input type="text" class="missing" name="email" bind:value={user.email} />

            {#if error.email}
                <p class="error-message">Email is required</p>
            {/if}
        </div>

        <div class="password">
            
            <label for="name">Password</label>
            <input
                type="password"
                id="password"
                name="password"
                bind:value={user.password}
                />

            {#if error.password}
                <p class="error-message">Password is required</p>
            {/if}
        </div>

        <button type="submit">add</button>
    </form>
</article>

<style>
    label
    {
        width: 20%;
    }
    
    input
    {
        -webkit-padding: 0.1em 0.5em;
        padding: 0.1em 0.5em;
        box-sizing: border-box;
        margin: 0em 1em;
        width: 70%;
        color: black;
    }

	.contact-card {
		width: auto;
        height: auto;
		border: 1px solid #aaa;
		border-radius: 2px;
		box-shadow: 2px 2px 8px rgba(0,0,0,0.1);
		padding: 1em;
        background-color: #666;
	}

	h2 {
		padding: 0 0 0.2em 0;
		margin: 1 1 1em 0;
		border-bottom: 1px solid #ff3e00
	}

	.host, .password, .email {
		width: auto;

		padding: 0 0 0 1.5em;
		background:  0 50% no-repeat;
		background-size: 1em 1em;
		margin: 0 0 0.5em 0em;
		line-height: 1.2;
	}

    form > div {
        display: flex;
        flex-wrap: wrap;
    }

    /* ... */

    .error-message {
        color: tomato;
        flex: 0 0 100%;
        margin: 0 2px;
        font-size: 0.8em;
    }
    
	.missing {
		color: #111;
	}

    * {
      box-sizing: border-box;
    }
    form {
      display: flex;
      flex-direction: column;
    }

    form > div + * {
      margin-top: 10px;
    }


    .exit {
        position: absolute;
        right: 20px;
        top: 15px;
        width: 32px;
        height: 32px;
        opacity: 0.3;
    }
    
    .exit:hover {
        opacity: 1;
    }

    .exit:before, .exit:after {
        position: absolute;
        left: 15px;
        content: ' ';
        height: 33px;
        width: 2px;
        background-color: #333;
    }
    
    .exit:before {
        transform: rotate(45deg);
    }
    
    .exit:after {
        transform: rotate(-45deg);
    }

</style>
