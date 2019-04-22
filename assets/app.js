function textNodesUnder(node){
    var all = [];
    for (node=node.firstChild;node;node=node.nextSibling){
        if (node.nodeType==3) all.push(node);
        else all = all.concat(textNodesUnder(node));
    }
    return all;
}

var exTerminal = undefined
var anims = []
var speed = 1
var gen = function(node,text){return function(){node.data = text}}
function typeIn(el) {
    var nodes = textNodesUnder(el)
    for (var i = 0; i < nodes.length; i++) if(!nodes[i].done) {
        nodes[i].done = true
        data = nodes[i].data.split('')
        var noise = nodes[i].data.replace(/[^\s]/gi," ")
        nodes[i].data = noise
        for (var pos = 0; pos < data.length; pos+=speed)
            anims.push(gen(nodes[i],data.slice(0,pos+1).join('') + noise.substr(0,data.length - pos - 1)))
        anims.push(gen(nodes[i],data.join('')))
    }
}
setInterval( function(){ 
    if (anims.length) {
        anims.shift()() 
        //if (exTerminal) exTerminal.pause()
    } else {
        //if (exTerminal) exTerminal.resume()
    }
    speed -= 10
    if (speed < 0 ) speed = 0
},0)

jQuery(function($){
    $("body").append("<div id='terminal' class='tbody'></div>");

    $(document).on('click', 'a', function(e){
        e.preventDefault()
        var href = $(this).attr('href')
        if (href.startsWith("#")) {
            if (href.startsWith("#!")) {
                if (exTerminal) exTerminal.exec(href.substr(2))
                return
            } else return
        }
        if (exTerminal) exTerminal.exec("! " + href,true)
    })

    $(document).on("mouseover","a",function(e){
        e.preventDefault()
        var href = $(this).attr('href')
        if (href.startsWith("#")) {
            if (href.startsWith("#!")) {
                if (exTerminal) exTerminal.set_command(href.substr(2))
                return
            } else return
        }
        if (exTerminal) exTerminal.set_command("! " + href,true)
    })

    $("#terminal").terminal(function(cmd,term){
        var args = cmd.split(/\s/)
        switch(args[0]) {
            case "": return
            case "go":
            case "run":
            case "load":
            case "открыть":
            case "!":
            case "+" :
                //term.pause()
                $.get(args[1]+" #source",function(html){
                    term.echo(html,{raw:true})
                }).fail(function(){
                    window.open(args[1])
                })
            break 
            case "log": 
            case "git": 
            case "гит": 
            case "лог": 
            $.get("https://api.github.com/repos/fonline-roleplay/FO4RP/commits", function(result) {
                for (var i in result) {
                    term.echo("<div><a href='"+result[i].html_url+"'>"+(new Date(result[i].commit.author.date).toLocaleDateString().replace(/\./g,'/'))+" - "+result[i].author.login+"</a>: "+result[i].commit.message.replace(/\n+/g,"<br>")+"</div>", {raw:true})
                }
            })
            break
            case "echo":
                term.echo(cmd)
                break
            default: 
                term.echo("ОШИБКА: Неизвестная команда <"+args[0]+">")
            break
        }
    }, {
        greetings: '',
        name: 'terminal',
        prompt: 'guest@poseidon:net/> ',
        scrollOnEcho: false,
        onInit: function(term) {
            exTerminal = term
            
            var fn = term.echo
            term.echo = function() {
                var res = fn.apply(term, arguments)
                if (arguments[0].indexOf(term.get_prompt()) < 0) {
                    speed += 10

                    $("div[data-index]").each(function(i,el){
                        $(el).removeAttr('data-index')
                        typeIn(el)
                    })
                }
                return res
            }

            term.echo($("#source").html(),{raw:true})

            $('#terminal').unbind('scroll')
        }
    })
})