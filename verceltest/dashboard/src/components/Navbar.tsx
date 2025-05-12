import { Link } from "react-router-dom";

const Navbar: React.FC = () => (
  <nav className="bg-[#10302B] text-white w-full px-6 py-2 flex items-center justify-between shadow h-16 min-h-0">
    {/* Logo as clickable link */}
    <div className="flex items-center gap-4">
      <Link to="/">
        <img
          src="/pax.logo.png"
          alt="Pax logo"
          className="w-20 h-12 object-contain hover:opacity-80 transition"
        />
      </Link>
    </div>
    {/* Navigation Links */}
    <ul className="flex gap-8 items-center">
      <li>
        <Link to="/stats" className="block py-2 hover:underline">
          Statistik
        </Link>
      </li>
      <li>
        <Link to="/users" className="block py-2 hover:underline">
          Användare
        </Link>
      </li>
      <li>
        <Link to="/auth" className="block py-2 hover:underline ml-4">
          Logga in
        </Link>
      </li>
    </ul>
  </nav>
);

export default Navbar;
