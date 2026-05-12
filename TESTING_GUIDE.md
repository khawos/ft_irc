# Guide de Test IRC - Multiples Clients IRSSI

## Configuration de Test

- **Serveur**: `localhost:6667` (ou le port configuré)
- **Clients**: Minimum 3 clients IRSSI pour les tests complets
- **Client 1**: `admin` (sera opérateur)
- **Client 2**: `user1`
- **Client 3**: `user2`

---

## PHASE 1 : AUTHENTIFICATION ET CONNEXION

### Test 1.1 - Connexion avec mot de passe

**Client 1:**

```
/connect localhost 6667
/quote PASS motdepasseserveur
```

✓ Doit accepter la connexion
✗ Ne doit pas rejeter après PASS

### Test 1.2 - Définir un NICK

**Client 1:**

```
/nick admin
```

✓ Doit confirmer le changement de nick
✗ Pas d'erreur de duplication

### Test 1.3 - Définir l'USER

**Client 1:**

```
/quote USER admin 0 * :Administrator User
```

✓ Doit envoyer les messages de bienvenue
✓ Doit afficher les paramètres du serveur

### Test 1.4 - Connexion Client 2

**Client 2:**

```
/connect localhost 6667
/quote PASS motdepasseserveur
/nick user1
/quote USER user1 0 * :First User
```

✓ Connexion réussie

### Test 1.5 - Connexion Client 3

**Client 3:**

```
/connect localhost 6667
/quote PASS motdepasseserveur
/nick user2
/quote USER user2 0 * :Second User
```

✓ Connexion réussie

### Test 1.6 - Nick en double (Erreur)

**Client 1:**

```
/nick user1
```

✗ Doit recevoir erreur 433 (nick already in use)

### Test 1.7 - Changer de nick après authentification

**Client 2:**

```
/nick newuser1
```

✓ Doit changer le nick avec succès

---

## PHASE 2 : CANAUX - FONCTIONNALITÉS DE BASE

### Test 2.1 - Créer et rejoindre un canal

**Client 1:**

```
/join #general
```

✓ Admin crée le canal et devient opérateur
✓ Les autres clients voient: `:admin!admin@ip JOIN :#general`

### Test 2.2 - Rejoindre un canal existant

**Client 2:**

```
/join #general
```

✓ User1 rejoint le canal
✓ Tous les clients reçoivent la notification JOIN

### Test 2.3 - Rejoindre (Client 3)

**Client 3:**

```
/join #general
```

✓ User2 rejoint le canal
✓ Les 3 clients sont dans #general

### Test 2.4 - Voir la liste des membres

**Client 2:**

```
/names #general
```

✓ Affiche: `admin`, `user1`, `user2`
✓ Affiche `@admin` (opérateur) avec le préfixe `@`

### Test 2.5 - Voir le sujet du canal (vide au départ)

**Client 1:**

```
/topic #general
```

✓ Message 331: No topic is set

---

## PHASE 3 : MESSAGES

### Test 3.1 - Message dans le canal

**Client 1:**

```
Hello everyone!
```

✓ Tous les clients voient: `:admin!admin@ip PRIVMSG #general :Hello everyone!`
✓ Le client 1 n'a pas l'écho

### Test 3.2 - Message privé (Client 2 à Client 1)

**Client 2:**

```
/msg admin Bonjour admin
```

✓ Client 1 reçoit le message privé
✓ Format: `:user1!user1@ip PRIVMSG admin :Bonjour admin`

### Test 3.3 - Message privé (Client 1 répond)

**Client 1:**

```
/msg user1 Salut user1!
```

✓ Client 2 reçoit la réponse

### Test 3.4 - Message dans canal (Client 3)

**Client 3:**

```
Coucou le groupe!
```

✓ Tous les clients reçoivent le message

### Test 3.5 - Message à un client non existant

**Client 1:**

```
/msg nosuchuser Coucou
```

✗ Reçoit erreur 401 (No such nick)

---

## PHASE 4 : COMMANDES D'OPÉRATEUR - MODE

### Test 4.1 - Vérifier les droits d'opérateur

**Client 2 (user1):**

```
/mode #general +i
```

✗ Doit recevoir erreur 482 (You're not channel operator)

### Test 4.2 - Mode +i (Invite-only)

**Client 1 (admin/opérateur):**

```
/mode #general +i
```

✓ Le canal devient invite-only
✓ Tous les membres reçoivent: `:ircserv MODE #general +i`

### Test 4.3 - Rejoindre canal invite-only sans invitation

**Client 3 (nouveau terminal/client):**

```
/join #general
```

✗ Reçoit erreur 473 (Cannot join channel +i)

### Test 4.4 - Mode -i (Retirer invite-only)

**Client 1:**

```
/mode #general -i
```

✓ Le canal redevient public
✓ Notification envoyée à tous

### Test 4.5 - Mode +k (Channel key/password)

**Client 1:**

```
/mode #general +k supersecret
```

✓ Le canal nécessite un mot de passe
✓ Notification: `:ircserv MODE #general +k supersecret`

### Test 4.6 - Rejoindre canal protégé sans mot de passe

**Client 3 (nouveau):**

```
/join #general
```

✗ Erreur 475 (Cannot join channel +k)

### Test 4.7 - Rejoindre canal protégé avec bon mot de passe

**Client 3:**

```
/join #general supersecret
```

✓ Client 3 rejoint le canal

### Test 4.8 - Rejoindre avec mauvais mot de passe

**Nouveau client:**

```
/join #general mauvaismdp
```

✗ Erreur 475 (Cannot join channel +k)

### Test 4.9 - Mode -k (Retirer password)

**Client 1:**

```
/mode #general -k
```

✓ Le canal ne nécessite plus de mot de passe

### Test 4.10 - Mode +t (Topic réservé aux opérateurs)

**Client 1:**

```
/mode #general +t
```

✓ Seuls les opérateurs peuvent changer le sujet

### Test 4.11 - Mode -t (Permettre à tous de changer le sujet)

**Client 1:**

```
/mode #general -t
```

✓ N'importe qui peut changer le sujet

### Test 4.12 - Mode +o (Promouvoir opérateur)

**Client 1:**

```
/mode #general +o user1
```

✓ User1 devient opérateur
✓ Notification: `:admin!admin@ip MODE #general +o user1`
✓ La liste des noms affiche `@user1`

### Test 4.13 - Mode -o (Retirer opérateur)

**Client 1:**

```
/mode #general -o user1
```

✓ User1 redevient utilisateur normal
✓ Notification: `:admin!admin@ip MODE -o user1`

### Test 4.14 - Mode +l (Limite d'utilisateurs)

**Client 1:**

```
/mode #general +l 2
```

✓ Maximum 2 utilisateurs dans le canal

### Test 4.15 - Rejoindre canal plein

**Nouveau client:**

```
/join #general
```

✗ Erreur 471 (Cannot join channel +l)

### Test 4.16 - Mode -l (Retirer limite)

**Client 1:**

```
/mode #general -l
```

✓ Limite supprimée

---

## PHASE 5 : COMMANDES D'OPÉRATEUR - TOPIC

### Test 5.1 - Définir un sujet (Client opérateur)

**Client 1 (avec +t activé):**

```
/topic #general Bienvenue sur #general!
```

✓ Sujet changé
✓ Notification: `:admin!admin@ip TOPIC #general :Bienvenue sur #general!`

### Test 5.2 - Voir le sujet

**Client 2:**

```
/topic #general
```

✓ Affiche le sujet: `Bienvenue sur #general!`

### Test 5.3 - Non-opérateur tente de changer le sujet (+t actif)

**Client 2:**

```
/topic #general Nouveau sujet
```

✗ Doit recevoir une erreur (pas opérateur)

### Test 5.4 - Retirer le mode +t et changer le sujet

**Client 1:**

```
/mode #general -t
```

✓ Mode retiré

**Client 2:**

```
/topic #general Sujet changé par user1
```

✓ Sujet changé avec succès
✓ Tous reçoivent la notification

---

## PHASE 6 : COMMANDES D'OPÉRATEUR - KICK

### Test 6.1 - Kick par non-opérateur (Erreur)

**Client 2:**

```
/kick #general user2
```

✗ Erreur 482 (You're not channel operator)

### Test 6.2 - Kick par opérateur

**Client 1:**

```
/kick #general user2 Spam
```

✓ User2 est expulsé
✓ Tous reçoivent: `:admin!admin@ip KICK #general user2 :Spam`
✓ User2 est automatiquement parti du canal

### Test 6.3 - User2 essaie de rejoindre après kick

**Client 3 (user2):**

```
/join #general
```

✓ Peut rejoindre (pas de ban permanent)

### Test 6.4 - Kick avec raison vide

**Client 1:**

```
/kick #general user2
```

✓ Utilise la raison par défaut si applicable

### Test 6.5 - Kick d'un utilisateur non existant

**Client 1:**

```
/kick #general nosuchuser
```

✗ Erreur 401 (No such nick)

### Test 6.6 - Kick d'un utilisateur qui n'est pas dans le canal

**Client 1:**

```
/kick #general user1 (où user1 n'est pas dans #general)
```

✗ Erreur 441 (They aren't on that channel)

---

## PHASE 7 : COMMANDES D'OPÉRATEUR - INVITE

### Test 7.1 - Activer le mode invite-only

**Client 1:**

```
/mode #general +i
```

✓ Mode +i activé

### Test 7.2 - Inviter un utilisateur

**Client 1:**

```
/invite user2 #general
```

✓ User2 reçoit la notification d'invitation
✓ Format: `:admin!admin@ip INVITE user2 :#general`

### Test 7.3 - Rejoindre après invitation

**Client 3 (user2):**

```
/join #general
```

✓ Rejoint sans erreur
✓ L'invitation a permis l'accès au canal +i

### Test 7.4 - Inviter un utilisateur déjà dans le canal (Erreur)

**Client 1:**

```
/invite user2 #general
```

✗ Erreur 443 (is already on channel)

### Test 7.5 - Inviter un utilisateur inexistant

**Client 1:**

```
/invite nosuchuser #general
```

✗ Erreur 401 (No such nick)

### Test 7.6 - Inviter en mode -i (pas invite-only)

**Client 1:**

```
/mode #general -i
/invite user1 #general
```

✓ L'invitation fonctionne même en mode public

### Test 7.7 - Non-opérateur invite en mode +i (Erreur)

**Client 1 (retirer opérateur à user1):**

```
/mode #general -o user1
```

**Client 2 (user1, non-op):**

```
/invite user3 #general
```

✗ Erreur 482 (You're not channel operator)

---

## PHASE 8 : STRESS TEST - MULTIPLES CANAUX

### Test 8.1 - Client dans plusieurs canaux

**Client 1:**

```
/join #channel1
/join #channel2
/join #channel3
```

✓ Admin est dans 3 canaux

### Test 8.2 - Message dans chaque canal

**Client 1:**

```
/msg #channel1 Message 1
/msg #channel2 Message 2
/msg #channel3 Message 3
```

✓ Chaque canal reçoit son message

### Test 8.3 - Quitter un canal

**Client 1:**

```
/part #channel1
```

✓ Admin quitte #channel1
✓ Les autres reçoivent la notification PART

### Test 8.4 - Créer un canal avec user2

**Client 3:**

```
/join #private
```

✓ User2 est opérateur de #private

### Test 8.5 - User1 rejoint #private

**Client 2:**

```
/join #private
```

✓ Rejoint le canal

---

## PHASE 9 : GESTION DE DÉCONNEXION

### Test 9.1 - Déconnexion propre

**Client 2:**

```
/quit
```

✓ Client 2 se déconnecte
✓ Les autres clients reçoivent: `:user1!user1@ip QUIT :...`
✓ User1 est retiré de tous les canaux

### Test 9.2 - Déconnexion brutale

**Client 3 (fermer le terminal):**

```
(Fermer la fenêtre)
```

✓ Le serveur détecte la déconnexion
✓ Les autres clients reçoivent la notification de départ
✓ User2 est retiré des canaux

### Test 9.3 - Reconnexion

**Client 2 (reconnectez-vous):**

```
/connect localhost 6667
/quote PASS motdepasseserveur
/nick user1
/quote USER user1 0 * :First User
```

✓ Reconnexion réussie avec nouveau nick (ou un nick libre)

---

## PHASE 10 : CAS LIMITES

### Test 10.1 - Nom de canal invalide

**Client 1:**

```
/join invalidchannel (sans #)
```

Comportement: À vérifier (certains serveurs rejettent, d'autres acceptent avec #)

### Test 10.2 - Channel avec caractères spéciaux

**Client 1:**

```
/join #channel-test_123
```

✓ Doit fonctionner

### Test 10.3 - Message très long

**Client 1:**

```
/msg #general [MESSAGE LONG DE 512+ CARACTÈRES]
```

Comportement: À vérifier

### Test 10.4 - Commande invalide

**Client 1:**

```
/quote INVALIDCMD param1 param2
```

Comportement: Pas de crash du serveur

### Test 10.5 - Trop de clients simultanés

Lancer 10+ connexions
✓ Le serveur gère sans hang ni crash

---

## RÉSUMÉ DES VÉRIFICATIONS ESSENTIELLES

| Fonctionnalité   | Test                                   | Résultat | Notes                |
| ---------------- | -------------------------------------- | -------- | -------------------- |
| Authentification | PASS + NICK + USER                     | ✓        | Client connecté      |
| Canaux           | JOIN + NAMES + PART                    | ✓        | Navigation fluide    |
| Messages         | PRIVMSG (#channel, @user)              | ✓        | Pas d'écho au sender |
| Opérateurs       | MODE +o/-o                             | ✓        | Préfixe @ visible    |
| KICK             | MODE + KICK                            | ✓        | Expulsion de canal   |
| INVITE           | MODE +i + INVITE                       | ✓        | Accès conditionnel   |
| TOPIC            | TOPIC + MODE +t/-t                     | ✓        | Restriction OPs      |
| MODE             | +i, -i, +k, -k, +t, -t, +o, -o, +l, -l | ✓        | Tous les flags       |
| Déconnexion      | QUIT + détection                       | ✓        | Nettoyage propre     |
| Stress           | 5+ clients, 3+ canaux                  | ✓        | Pas de leaks         |

---

## COMMANDES IRSSI PRATIQUES

```
# Connexion
/connect localhost 6667

# Navigation
/join #channel
/part #channel
/quit

# Messages
/msg #channel message
/msg user message

# Mode
/mode #channel +flags user

# Informations
/names #channel
/topic #channel
/whois user

# Debug
/quote COMMANDE param1 param2
```

---

## NOTES D'EXÉCUTION

1. **Terminal 1**: Démarrer le serveur

   ```bash
   ./ircserv 6667 motdepasseserveur
   ```

2. **Terminals 2, 3, 4**: Lancer IRSSI

   ```bash
   irssi
   ```

3. **Dans IRSSI**, se connecter:

   ```
   /connect localhost 6667
   /quote PASS motdepasseserveur
   /nick admin
   /quote USER admin 0 * :Admin
   ```

4. **Observer les logs serveur** pour vérifier:
   - Pas de messages de debug
   - Pas de leaks mémoire
   - Pas de segfault

---

**Dernière mise à jour**: 12/05/2026
**Status**: À tester entièrement
