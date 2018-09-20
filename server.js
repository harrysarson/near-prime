const Koa = require('koa');
const Router = require('koa-router');
const bodyParser  = require('koa-bodyparser');
const cors = require('@koa/cors');
const Debug = require('debug');
const  { prime_search } = require('bindings')('native_prime_search.node')

const app = new Koa();
const router = new Router();
const debug = Debug('near-prime');

router.post('/prime_search', async (ctx, next) => {
    await next();
    let non_prime = ctx.request.body;
    let truelyPrime, prime = '';
    try {
        debug(`non prime: ${non_prime}`);
        [ truelyPrime, prime ] = prime_search(non_prime);
        debug(`is truely prime? ${truelyPrime}`);
        debug(`non prime: ${prime}`);
    } catch(e) {
        console.error('Error processing request!');
        console.error(ctx);
        console.error(e);
        
        ctx.status = err.status || 500;
        ctx.app.emit('error', err, ctx);
    }
    ctx.body = prime;
});


app
    .use(async (ctx, next) => {
        ctx.set('Access-Control-Allow-Origin', '*');
        ctx.set('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
        await next();
    })
    .use(router.routes())
    .use(bodyParser({
        enableTypes: [ 'text' ],
        textLimit: [ '1kb' ]
    }))


app.listen(3000);